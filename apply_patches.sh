#!/bin/bash

# Script to apply patches from the patches/ directory to the corresponding src/ directories
# Naming convention: patches/{org}_{package}.patch -> src/{org}/{package}/

# Don't use set -e here because we handle errors explicitly
set -o pipefail

PATCHES_DIR="patches"
SRC_DIR="src"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd "$SCRIPT_DIR"

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if patches directory exists
if [ ! -d "$PATCHES_DIR" ]; then
    echo -e "${RED}Error: patches/ directory not found${NC}"
    exit 1
fi

# Count patches
patch_count=$(find "$PATCHES_DIR" -name "*.patch" -type f | wc -l)

if [ "$patch_count" -eq 0 ]; then
    echo -e "${YELLOW}No patches found in patches/ directory${NC}"
    exit 0
fi

echo -e "${BLUE}Found $patch_count patch(es) to apply${NC}"
echo ""

# Track successes and failures
success_count=0
fail_count=0
skip_count=0
failed_patches=()

# Iterate through all .patch files in patches directory
for patch_file in "$PATCHES_DIR"/*.patch; do
    # Get just the filename without path
    patch_name=$(basename "$patch_file")

    # Remove .patch extension
    base_name="${patch_name%.patch}"

    # Split by underscore to get org and package
    # Format: {org}_{package}.patch
    org=$(echo "$base_name" | cut -d'_' -f1)
    package=$(echo "$base_name" | cut -d'_' -f2-)

    # Construct target directory
    target_dir="$SRC_DIR/$org/$package"

    echo -e "${BLUE}Processing:${NC} $patch_name"
    echo -e "  Target: $target_dir"

    # Check if target directory exists
    if [ ! -d "$target_dir" ]; then
        echo -e "  ${RED}✗ Target directory does not exist${NC}"
        echo ""
        ((fail_count++))
        failed_patches+=("$patch_name (target not found)")
        continue
    fi

    # Check if patch is already applied by doing a dry-run with --reverse
    if git -C "$target_dir" apply --reverse --check "$SCRIPT_DIR/$patch_file" &>/dev/null; then
        echo -e "  ${YELLOW}⊙ Already applied (skipping)${NC}"
        echo ""
        ((skip_count++))
        continue
    fi

    # Try to apply the patch
    if git -C "$target_dir" apply --check "$SCRIPT_DIR/$patch_file" &>/dev/null; then
        # Dry-run succeeded, now apply for real
        if git -C "$target_dir" apply "$SCRIPT_DIR/$patch_file"; then
            echo -e "  ${GREEN}✓ Successfully applied${NC}"
            ((success_count++))
        else
            echo -e "  ${RED}✗ Failed to apply${NC}"
            ((fail_count++))
            failed_patches+=("$patch_name")
        fi
    else
        # Try with patch command as fallback
        echo -e "  ${YELLOW}⚠ git apply failed, trying patch command...${NC}"
        if patch -d "$target_dir" -p1 --dry-run < "$SCRIPT_DIR/$patch_file" &>/dev/null; then
            if patch -d "$target_dir" -p1 < "$SCRIPT_DIR/$patch_file"; then
                echo -e "  ${GREEN}✓ Successfully applied with patch${NC}"
                ((success_count++))
            else
                echo -e "  ${RED}✗ Failed to apply with patch${NC}"
                ((fail_count++))
                failed_patches+=("$patch_name")
            fi
        else
            echo -e "  ${RED}✗ Cannot apply (conflicts or already modified)${NC}"
            ((fail_count++))
            failed_patches+=("$patch_name (conflicts)")
        fi
    fi

    echo ""
done

# Print summary
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo -e "${BLUE}Summary:${NC}"
echo -e "  ${GREEN}✓ Applied:${NC} $success_count"
echo -e "  ${YELLOW}⊙ Skipped (already applied):${NC} $skip_count"
echo -e "  ${RED}✗ Failed:${NC} $fail_count"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if [ "$fail_count" -gt 0 ]; then
    echo -e "\n${RED}Failed patches:${NC}"
    for failed in "${failed_patches[@]}"; do
        echo -e "  - $failed"
    done
    exit 1
fi

if [ "$success_count" -gt 0 ]; then
    echo -e "\n${GREEN}All patches applied successfully!${NC}"
fi

exit 0
