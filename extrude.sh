NekMesh -m extrude:layers=32 outerRegion.xml 3Dout.xml
NekMesh -m extrude:layers=7 inFoil.xml 3Din.xml
NekMesh 3Din.xml 3DinUcomp.xml:xml:uncompress
NekMesh 3Dout.xml 3DoutUcomp.xml:xml:uncompress
