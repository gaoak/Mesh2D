setPlotParameters
%% load files
flat = load('flat10');
vall = load('vally87');
nf = length(flat);
nv = length(vall);
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
Height = ds;
file = fopen('domain.geo', 'w');
Nlid = round(wavelength*NP/ds);
%points
for kk=1:1:NP
    for ii=1:1:nd
        if kk>1 && ii==1
            continue
        end
        fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, data(ii,1) + wavelength*(kk-1), data(ii,2), data(ii,3), 2*ds);
        count = count + 1;
    end
end
dstmp = wavelength*NP/Nlid;
for ii=1:1:(Nlid+1)
    fprintf(file, 'Point(%d) = { %f, %f, %f, %f};\n', count, wavelength*NP - dstmp * (ii-1), Height, 0., 2*ds);
    count = count + 1;
end
PtsNum = count - 1;
%line
LineNumStart = count;
for ii=1:1:PtsNum-1
    
    fprintf(file, 'Line(%d) = { %d, %d};\n', count, ii, ii+1);
    count = count + 1;
end
fprintf(file, 'Line(%d) = { %d, %d};\n', count, PtsNum, 1);
count = count + 1;
LineNum = PtsNum;
%line loop
fprintf(file, 'Line Loop(%d) = {', count);
for ii=(1:1:LineNum-1) + LineNumStart - 1
    fprintf(file, '%d, ', ii);
end
fprintf(file, '%d};\n', LineNum + LineNumStart - 1);
count = count + 1;
%plane Surface
fprintf(file, 'Plane Surface(%d) = {%d};\n', count, count - 1);
fprintf(file, 'Recombine Surface {%d};\n', count);
count = count + 1;
fprintf(file, 'Mesh.RecombinationAlgorithm = 0;\n');
fclose(file);