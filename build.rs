fn main() {
    let mut build = cxx_build::bridge("src/main.rs");

    // Include Ice headers and generated Slice bindings
    build
        .std("c++17")
        .file("src/cxx/MumbleServer.cpp")
        .file("src/cxx/ice_helper.cpp")
        .include("/opt/homebrew/include/Ice")
        .include("/opt/homebrew/Cellar/ice/3.7.10/include")
        .include("src/cxx")
        .include("slice") // Folder where the generated headers are located
        .compile("mumble_ice_integration");

    // Link with the Ice library
    println!("cargo:rustc-link-lib=Ice");
    println!("cargo:rustc-link-search=/opt/homebrew/Cellar/ice/3.7.10/lib");
    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=src/cxx/MumbleServer.cpp");
    println!("cargo:rerun-if-changed=src/cxx/MumbleServer.h");
    println!("cargo:rerun-if-changed=src/cxx/ice_helper.h");
    println!("cargo:rerun-if-changed=src/cxx/ice_helper.cpp");
}
