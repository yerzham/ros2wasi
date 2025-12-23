#ifndef RMW_WASM_COMPONENT_CPP_RMW_SUBSCRIPTION_DATA_HPP
#define RMW_WASM_COMPONENT_CPP_RMW_SUBSCRIPTION_DATA_HPP

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <mutex>
#include <cstdint>

namespace rmw_wasm_component_cpp {

/// Data stored in subscription->data
struct SubscriptionData {
    std::string topic_name;
    std::deque<std::vector<uint8_t>> message_queue;

    /// Push a message to the queue
    void enqueue(std::vector<uint8_t> message) {
        message_queue.push_back(std::move(message));
    }

    /// Pop a message from the queue, returns empty vector if none
    std::vector<uint8_t> dequeue() {
        if (message_queue.empty()) {
            return {};
        }
        auto msg = std::move(message_queue.front());
        message_queue.pop_front();
        return msg;
    }

    /// Check if there are messages available
    bool has_message() const {
        return !message_queue.empty();
    }
};

/// Global subscription registry for routing incoming messages
class SubscriptionRegistry {
public:
    static SubscriptionRegistry& instance() {
        static SubscriptionRegistry registry;
        return registry;
    }

    void register_subscription(const std::string& topic, SubscriptionData* sub) {
        subscriptions_[topic].push_back(sub);
    }

    void unregister_subscription(const std::string& topic, SubscriptionData* sub) {
        auto it = subscriptions_.find(topic);
        if (it != subscriptions_.end()) {
            auto& subs = it->second;
            subs.erase(std::remove(subs.begin(), subs.end(), sub), subs.end());
            if (subs.empty()) {
                subscriptions_.erase(it);
            }
        }
    }

    /// Route a message to all subscriptions for a topic
    void route_message(const std::string& topic, const std::vector<uint8_t>& data) {
        auto it = subscriptions_.find(topic);
        if (it != subscriptions_.end()) {
            for (auto* sub : it->second) {
                sub->enqueue(data);
            }
        }
    }

private:
    SubscriptionRegistry() = default;
    std::unordered_map<std::string, std::vector<SubscriptionData*>> subscriptions_;
};

}  // namespace rmw_wasm_component_cpp

#endif  // RMW_WASM_COMPONENT_CPP_RMW_SUBSCRIPTION_DATA_HPP
