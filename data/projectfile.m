setPlotParameters
%% load files
flat = load('flat10');
vall = load('vally87');
nf = length(flat);
nv = length(vall);
%figure;
%plot3(flat(:,1),flat(:,2),flat(:,3))
%hold on;
%plot3(vall(:,1),vall(:,2),vall(:,3))
%hold off;
%% transform data
planf = project(flat);
planv = project(vall);
planf(:,:) = planf(nf:-1:1,:);

Nhalf = round(nf/2);
p0 = planf(Nhalf, :);

planf(:,1) = planf(:,1)-p0(1);
planf(:,2) = planf(:,2)-p0(2);
planv(:,1) = planv(:,1)-p0(1);
planv(:,2) = planv(:,2)-p0(2);
scale = 0.001;
planf = planf *  scale;
planv = planv *  scale;
planfaft = planf(2:1:Nhalf,:);
planfaft(:,1) = planfaft(:,1) - planf(1,1) + planv(nv,1);
data = [planf(Nhalf:1:(nf-1),:); planv; planfaft];
nd = length(data)
%% wavelength
wavelength = data(nd, 1);
%% plot
figure;
plot(data(:,1), data(:,2), '.-b')

%% output gmsh file
count = 1;
NP = 10;
ds = 4.6000e-04;
file = fopen('domain.geo', 'w');
%%%%%%%%%%%%%%%%%%%%%%%line loop 1
Height = 0.1;
%bottom points, gear surface
lines1=[];
for kk=1:1:NP
    for ii=1:1:nd
        if kk>1 && ii==1
            continue
        end
        fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, data(ii,1) + wavelength*(kk-1), data(ii,2), data(ii,3), 2*ds);
        lines1 = [lines1; count-1 count];
        count = count + 1;
    end
end
%right side
segments = BoundayLayer(ds, 1.1, 0.01,Height);
lenseg = length(segments);
for ii=2:1:lenseg-1
    fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, wavelength*NP, segments(ii), 0., 2*(segments(ii)-segments(ii-1)));
    lines1 = [lines1; count-1 count];
    count = count + 1;
end
%upper lid
Nlid = round(wavelength*NP/0.01);
dstmp = wavelength*NP/Nlid;
for ii=1:1:(Nlid+1)
    fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, wavelength*NP - dstmp * (ii-1), segments(lenseg), 0., 0.01);
    lines1 = [lines1; count-1 count];
    count = count + 1;
end
%left side
for ii=lenseg-1:-1:2
    fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, 0, segments(ii), 0., 2*(segments(ii)-segments(ii-1)));
    lines1 = [lines1; count-1 count];
    count = count + 1;
end
lines1(1,1) = count - 1;
%%%%%%%%%%%%%%%%%%%%%%%%%
%tool parameters
tx = wavelength*NP*0.455;
ty = 0.03;
lines2=[];
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx+0.004, ty, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx+0.004, ty-0.03, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx+0.008, ty-0.03, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx+0.0025, ty-0.04, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx-0.0025, ty-0.04, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx-0.008, ty-0.03, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx-0.004, ty-0.03, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, tx-0.004, ty, 0., 2*ds);
lines2 = [lines2; count-1 count];
count = count + 1;
lines2(1,1) = count - 1;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%nozzle
tx = wavelength*NP*0.5;
ty = 0.03;
vect1 = 0.005*[cos(-40/180*pi), sin(-40/180*pi)];
vect2 = 0.006*[cos(50/180*pi),  sin(50/180*pi)];
lines3=[];
nozzlex = tx+0.01;
nozzley = ty;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, nozzlex, nozzley, 0., 2*ds);
lines3 = [lines3; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, nozzlex+vect2(1), nozzley+vect2(2), 0., 2*ds);
lines3 = [lines3; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, nozzlex+vect1(1)+vect2(1), nozzley+vect1(2)+vect2(2), 0., 2*ds);
lines3 = [lines3; count-1 count];
count = count + 1;
fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, nozzlex+vect1(1), nozzley+vect1(2), 0., 2*ds);
lines3 = [lines3; count-1 count];
count = count + 1;
lines3(1,1) = count - 1;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%lines 1,2,3
loop1=[];
for ii=1:1:length(lines1)
    fprintf(file, 'Line(%d) = { %d, %d};\n', count, lines1(ii,1), lines1(ii,2));
    loop1 = [loop1; count];
    count = count + 1;
end
loop2=[];
for ii=1:1:length(lines2)
    fprintf(file, 'Line(%d) = { %d, %d};\n', count, lines2(ii,1), lines2(ii,2));
    loop2 = [loop2; count];
    count = count + 1;
end
loop3=[];
for ii=1:1:length(lines3)
    fprintf(file, 'Line(%d) = { %d, %d};\n', count, lines3(ii,1), lines3(ii,2));
    loop3 = [loop3; count];
    count = count + 1;
end
%line loop
surfs=[];
fprintf(file, 'Line Loop(%d) = {', count);
for ii=1:length(loop1)-1
    fprintf(file, '%d, ', loop1(ii));
end
fprintf(file, '%d};\n', loop1(length(loop1)));
surfs = [surfs; count];
count = count + 1;
fprintf(file, 'Line Loop(%d) = {', count);
for ii=1:length(loop2)-1
    fprintf(file, '%d, ', loop2(ii));
end
fprintf(file, '%d};\n', loop2(length(loop2)));
surfs = [surfs; count];
count = count + 1;
fprintf(file, 'Line Loop(%d) = {', count);
for ii=1:length(loop3)-1
    fprintf(file, '%d, ', loop3(ii));
end
fprintf(file, '%d};\n', loop3(length(loop3)));
surfs = [surfs; count];
count = count + 1;
%plane Surface
fprintf(file, 'Plane Surface(%d) = {%d,%d,%d};\n', count, surfs(1),surfs(2),surfs(3));
fprintf(file, 'Recombine Surface {%d};\n', count);
count = count + 1;
fprintf(file, 'Mesh.RecombinationAlgorithm = 0;\n');
fclose(file);