for((i=0;i<4;++i))
do
rm surf${i}.plt
NekMesh -m extract:surf=${i} square.xml surf${i}.xml
FieldConvert surf${i}.xml surf${i}.plt
done
