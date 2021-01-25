@0x9eb64e19f86ee174;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("forest");

struct CapNode {
  id @0 :Int32;
  left @1 :Int32;
  right @2 :Int32;
  feature @3 :Float64;
  threshold @4 :Float64;
}

struct Forest {
  nodeList @0 :List(CapNode);
}
