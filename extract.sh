#!/bin/bash

for ((i=0; i<4; ++i))
do
    rm -f surf${i}.plt
    NekMesh -m extract:surf=${i} hartmann.xml surf${i}.xml
    FieldConvert surf${i}.xml surf${i}.plt
done
