//! Example of instantiating a wasm module which uses WASI imports.

/*
You can execute this example with:
    cmake examples/
    cargo run --example wasip2
*/

use std::collections::HashMap;

use wasmtime::component::{Component, HasSelf, Linker, Resource, ResourceTable, bindgen};
use wasmtime::*;
use wasmtime_wasi::{WasiCtx, WasiCtxView, WasiView, DirPerms, FilePerms};

bindgen!({
    world: "rmw",
    with: {
        "wasi:messaging/types.message": WasiMessage,
        "wasi:messaging/types.client": WasiMessagingClient
    }
});

pub struct ComponentRunStates {
    // These two are required basically as a standard way to enable the impl of IoView and
    // WasiView.
    // impl of WasiView is required by [`wasmtime_wasi::p2::add_to_linker_sync`]
    pub wasi_ctx: WasiCtx,
    pub resource_table: ResourceTable,
    // Messaging topic
    pub topic: String,
}

impl WasiView for ComponentRunStates {
    fn ctx(&mut self) -> WasiCtxView<'_> {
        WasiCtxView {
            ctx: &mut self.wasi_ctx,
            table: &mut self.resource_table,
        }
    }
}

// Implement messaging traits for ComponentRunStates
impl wasi::messaging::types::Host for ComponentRunStates {}

impl wasi::messaging::producer::Host for ComponentRunStates {
    fn send(
        &mut self,
        _c: wasmtime::component::Resource<wasi::messaging::producer::Client>,
        _topic: wasi::messaging::producer::Topic,
        _message: wasmtime::component::Resource<wasi::messaging::producer::Message>,
    ) -> std::result::Result<(), wasi::messaging::producer::Error> {
        Ok(())
    }
}

impl wasi::messaging::types::HostMessage for ComponentRunStates {
    fn new(&mut self, data: wasmtime::component::__internal::Vec<u8>) -> Resource<WasiMessage> {
        let id = self
            .resource_table
            .push(WasiMessage {
                data: data,
                metadata: HashMap::new(),
                content_type: "application/octet-stream".to_string(),
            })
            .unwrap();
        id
    }
    fn set_data(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
        data: wasmtime::component::__internal::Vec<u8>,
    ) -> () {
        let message = self.resource_table.get_mut(&message).unwrap();
        message.data = data;
    }
    fn data(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
    ) -> wasmtime::component::__internal::Vec<u8> {
        let message = self.resource_table.get_mut(&message).unwrap();
        message.data.clone()
    }
    fn set_metadata(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
        metadata: wasi::messaging::types::Metadata,
    ) -> () {
        let message = self.resource_table.get_mut(&message).unwrap();
        for meta in metadata {
            message.metadata.insert(meta.0, meta.1);
        }
    }
    fn add_metadata(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
        key: wasmtime::component::__internal::String,
        value: wasmtime::component::__internal::String,
    ) -> () {
        let message = self.resource_table.get_mut(&message).unwrap();
        message.metadata.insert(key, value);
    }
    fn remove_metadata(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
        key: wasmtime::component::__internal::String,
    ) -> () {
        let message = self.resource_table.get_mut(&message).unwrap();
        message.metadata.remove(&key);
    }
    fn metadata(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
    ) -> Option<wasi::messaging::types::Metadata> {
        let message = self.resource_table.get_mut(&message).unwrap();
        Some(
            message
                .metadata
                .iter()
                .map(|meta| (meta.0.clone(), meta.1.clone()))
                .collect(),
        )
    }
    fn set_content_type(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
        content_type: wasmtime::component::__internal::String,
    ) -> () {
        let message = self.resource_table.get_mut(&message).unwrap();
        message.content_type = content_type;
    }
    fn content_type(
        &mut self,
        message: wasmtime::component::Resource<WasiMessage>,
    ) -> Option<wasmtime::component::__internal::String> {
        let message = self.resource_table.get_mut(&message).unwrap();
        Some(message.content_type.clone())
    }
    fn topic(
        &mut self,
        _message: wasmtime::component::Resource<WasiMessage>,
    ) -> Option<wasi::messaging::types::Topic> {
        Some(self.topic.clone())
    }
    fn drop(&mut self, rep: wasmtime::component::Resource<WasiMessage>) -> wasmtime::Result<()> {
        self.resource_table.delete(rep)?;
        Ok(())
    }
}

impl wasi::messaging::types::HostClient for ComponentRunStates {
    fn connect(
        &mut self,
        name: wasmtime::component::__internal::String,
    ) -> std::result::Result<
        wasmtime::component::Resource<wasi::messaging::types::Client>,
        wasi::messaging::types::Error,
    > {
        println!("Connecting client: {}", name);
        let client = WasiMessagingClient {};
        if let Ok(client_res) = self.resource_table.push(client) {
            Ok(client_res)
        } else {
            Err(wasi::messaging::types::Error::Other(
                "Failed to create client resource on host".to_string(),
            ))
        }
    }
    fn disconnect(
        &mut self,
        _client: wasmtime::component::Resource<WasiMessagingClient>,
    ) -> std::result::Result<(), wasi::messaging::types::Error> {
        Ok(())
    }
    fn drop(
        &mut self,
        client: wasmtime::component::Resource<WasiMessagingClient>,
    ) -> wasmtime::Result<()> {
        self.resource_table.delete(client)?;
        Ok(())
    }
}

pub struct WasiMessage {
    data: Vec<u8>,
    metadata: HashMap<String, String>,
    content_type: String,
}

pub struct WasiMessagingClient {}

fn main() -> Result<()> {
    // Define the WASI functions globally on the `Config`.
    let mut config = Config::default();
    config.wasm_exceptions(true);
    config.wasm_threads(true);
    let engine: Engine = Engine::new(&config)?;
    let mut linker = Linker::new(&engine);
    wasmtime_wasi::p2::add_to_linker_sync(&mut linker)?;
    Rmw::add_to_linker::<_, HasSelf<_>>(&mut linker, |state| state)?;

    // Instantiate our component.
    let component = Component::from_file(&engine, "test.wasm")?;

    // Alternatively, instead of using `Command`, just instantiate it as a normal component
    // New states

    // Create a temporary directory for ROS logs
    use std::fs;
    let log_dir = std::env::temp_dir().join("ros_logs");
    fs::create_dir_all(&log_dir)?;

    let wasi = WasiCtx::builder()
        .inherit_stdio()
        .inherit_args()
        .env("ROS_LOG_DIR", "/tmp/ros_logs")
        .env("ROS_HOME", "/tmp/ros_home")
        .preopened_dir(
            log_dir,
            "/tmp/ros_logs",
            DirPerms::all(),
            FilePerms::all(),
        )?
        .build();
    let state = ComponentRunStates {
        wasi_ctx: wasi,
        resource_table: ResourceTable::new(),
        topic: String::new(),
    };
    let mut store = Store::new(&engine, state);
    // Instantiate it as a normal component
    let instance = linker.instantiate(&mut store, &component)?;
    // Get the index for the exported interface
    let interface_idx = instance
        .get_export_index(&mut store, None, "wasi:cli/run@0.2.0")
        .expect("Cannot get `wasi:cli/run@0.2.0` interface");
    // Get the index for the exported function in the exported interface
    let parent_export_idx = Some(&interface_idx);
    let func_idx = instance
        .get_export_index(&mut store, parent_export_idx, "run")
        .expect("Cannot get `run` function in `wasi:cli/run@0.2.0` interface");
    let func = instance
        .get_func(&mut store, func_idx)
        .expect("Unreachable since we've got func_idx");
    // As the `run` function in `wasi:cli/run@0.2.0` takes no argument and return a WASI result that correspond to a `Result<(), ()>`
    // Reference:
    // * https://github.com/WebAssembly/wasi-cli/blob/main/wit/run.wit
    // * Documentation for [Func::typed](https://docs.rs/wasmtime/latest/wasmtime/component/struct.Func.html#method.typed) and [ComponentNamedList](https://docs.rs/wasmtime/latest/wasmtime/component/trait.ComponentNamedList.html)
    let typed = func.typed::<(), (Result<(), ()>,)>(&store)?;
    let (result,) = typed.call(&mut store, ())?;
    // Required, see documentation of TypedFunc::call
    typed.post_return(&mut store)?;
    result.map_err(|_| anyhow::anyhow!("error"))
}
