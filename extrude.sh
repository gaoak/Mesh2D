NekMesh -m extrude:layers=2 outerRegion.xml 3Dout.xml
NekMesh -m extrude:layers=1 inFoil.xml      3Din.xml
NekMesh -m extrude:layers=2 wakeRegion.xml  3Dwake.xml
NekMesh 3Din.xml 3DinUcomp.xml:xml:uncompress
NekMesh 3Dout.xml 3DoutUcomp.xml:xml:uncompress
NekMesh 3Dwake.xml 3DwakeUcomp.xml:xml:uncompress
