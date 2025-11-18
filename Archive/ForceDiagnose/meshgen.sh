for((AoA=0;AoA<=20;++AoA))
do
sed -i '14d' params.h
sed -i -e "14i\  double AoA = ${AoA} / 180. * M_PI;" params.h
bash make.sh
mkdir -p /home/data3/Naca0012/Re1000/AoA${AoA}
cp build/outerRegion.xml /home/data3/Naca0012/Re1000/AoA${AoA}/airfoil.xml
done
