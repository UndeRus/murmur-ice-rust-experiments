use std::mem::ManuallyDrop;

use cxx::SharedPtr;
use cxx::CxxVector;
use cxx::WeakPtr;
use ffi::MumbleServerInstance;

#[cxx::bridge]
mod ffi {

    #[derive(Debug, Clone)]
    struct MumbleServerInstance {
        id: u8,
    }

    unsafe extern "C++" {
        include!("murmur-ice-rust-experiments/src/cxx/ice_helper.h");

        type Communicator;

        fn connect(self: &Communicator) -> UniquePtr<MetaProxy>;

        fn new_communicator() -> UniquePtr<Communicator>;

        type MetaProxy;

        fn get_servers(self: &MetaProxy) -> UniquePtr<CxxVector<MumbleServerInstance>>;
    }
}

fn main() {
    println!("Hello, world, CXX!");
    //ffi::example_function("test parameter");

    let c = ffi::new_communicator();
    let metaProxy = ManuallyDrop::new(c.connect());

    let servers = ManuallyDrop::new(metaProxy.get_servers());

    let result = servers.iter().map(|x|x.clone()).collect::<Vec<MumbleServerInstance>>();
    dbg!(result);
}
