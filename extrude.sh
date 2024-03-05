cp build/outerRegion.xml build/outerRegion_Otip.xml  build/inFoil.xml ./
NekMesh -m extrude:layers=60 outerRegion.xml 3DoutUcomp.xml:xml:uncompress
NekMesh -m extrude:layers=9  outerRegion_Otip.xml 3DoutUcomp2.xml:xml:uncompress
NekMesh -m extrude:layers=10 inFoil.xml 3DinUcomp.xml:xml:uncompress
cp 3D* ../Merge3D/
cp wallmapping.dat ../Merge3D/
