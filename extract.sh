for((i=0;i<=5;++i))
do
rm surf${i}.plt
NekMesh -m extract:surf=${i} outerRegion.xml surf${i}.xml
FieldConvert surf${i}.xml surf${i}.plt
done
