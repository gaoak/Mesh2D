cat ../params.h | awk '{if($2=="hFirstLayer") print $0}' > params3D.h
cat ../params.h | awk '{if($2=="wingn") print $0}' >> params3D.h
cat ../params.h | awk '{if($2=="tipn") print $0}' >> params3D.h
wingn=`cat ../params.h | awk '{if($2=="wingn") print $4}'`
tipn=`cat ../params.h | awk '{if($2=="tipn") print $4}'`
tipn1=$((tipn+1))
cp params3D.h ../../Merge3D/
NekMesh -m extrude:layers=${wingn} outerRegion.xml 3DoutUcomp.xml:xml:uncompress
NekMesh -m extrude:layers=${tipn}  outerRegion_Otip.xml 3DoutUcomp2.xml:xml:uncompress
NekMesh -m extrude:layers=${tipn1} inFoil.xml 3DinUcomp.xml:xml:uncompress
cp 3D* ../../Merge3D/
cp wallmapping.dat ../../Merge3D/