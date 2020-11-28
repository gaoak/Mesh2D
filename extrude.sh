wingn=$( cat params.h | awk '{if($2=="wingn") print $4}' )
tipn=$(cat params.h | awk '{if($2=="tipn") print $4}')
tipnplus1=$(($tipn+1))
echo $wingn, $tipn, $tipnplus1
NekMesh -m extrude:layers=$wingn outerRegion.xml 3DoutUcomp.xml:xml:uncompress
NekMesh -m extrude:layers=$tipn  outerRegion_Otip.xml 3DoutUcomp2.xml:xml:uncompress
NekMesh -m extrude:layers=$tipnplus1 inFoil.xml 3DinUcomp.xml:xml:uncompress
cp 3D* ../Merge3D/
cp wallmapping.dat ../Merge3D/
