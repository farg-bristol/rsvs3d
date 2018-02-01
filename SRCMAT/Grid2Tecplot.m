function [cellStr]=Grid2Tecplot(grid,fid)
    
    if numel(grid.volu)>0
        [cellStr]=Grid3DTecplot(grid);
    else
        [cellStr]=Grid2DTecplot(grid);
    end
    [cellStr]=PrepareCharForTecplot(cellStr);
    WriteToFile(cellStr,fid)
    fclose(fid);
end

%%
function [cellStr]=Grid2DTecplot(grid)
    
end

%% 3D Tecplot Data
function [cellStr]=Grid3DTecplot(grid)
    nVertDat=3;
    nCellDat=3;
    [cellHeader]=BuildPolyhedronHeader(grid,nVertDat,nCellDat);
    [cellData]=BuildPolyhedronDataBlock(grid,nVertDat,nCellDat);
    [cellFaceMap]=BuildPolyhedronFaceMap(grid);
    cellStr=[cellHeader,cellData,cellFaceMap];
end

function [cellHeader]=BuildPolyhedronHeader(grid,nVertDat,nCellDat)
    
    kk=0;
    kk=kk+1;cellHeader{kk}='VARIABLES = "X" ,"Y" , "Z" ,"v1" ,"v2", "v3"';
    kk=kk+1;cellHeader{kk}='ZONE';
    kk=kk+1;cellHeader{kk}='ZONETYPE = POLYHEDRON';
    kk=kk+1;cellHeader{kk}=['NODES = ',int2str(numel(grid.vert))];
    kk=kk+1;cellHeader{kk}=['ELEMENTS = ',int2str(numel(grid.volu))];
    kk=kk+1;cellHeader{kk}=['FACES = ',int2str(numel(grid.surf))];
    kk=kk+1;cellHeader{kk}=['TOTALNUMFACENODES = ',...
        int2str(sum(cellfun(@numel,{grid.surf.edgeind})))];
    kk=kk+1;cellHeader{kk}='NUMCONNECTEDBOUNDARYFACES = 0'; % unused
    kk=kk+1;cellHeader{kk}='TOTALNUMBOUNDARYCONNECTIONS = 0'; % unused
    
    kk=kk+1;cellHeader{kk}=sprintf('VARLOCATION=([%i-%i]=NODAL ,[%i-%i]=CELLCENTERED)]'...
        ,1,nVertDat,nVertDat+1,nVertDat+nCellDat);
    kk=kk+1;cellHeader{kk}='DATAPACKING=BLOCK';
    
    
    
end

function [cellData]=BuildPolyhedronDataBlock(grid,nVertDat,nCellDat)
    
    vertDat=vertcat(grid.vert.coord)';
    if size(vertDat,1)<nVertDat
        vertDat(nVertDat,end)=0;
    end
    
    voluDat=[grid.volu.fill];
    if size(voluDat,1)<nCellDat
        voluDat(nCellDat,end)=0;
    end
    
    cellData=cell([1,nVertDat+nCellDat]);
    
    for ii=1:nVertDat
        cellData{ii}=num2str(vertDat(ii,:),' %.16f');
    end
    for ii=1:nVertDat
        cellData{ii+nVertDat}=num2str(voluDat(ii,:),' %.16f');
    end
    
end

function [cellFaceMap]=BuildPolyhedronFaceMap(grid)
    
    cellFaceMap{1}=int2str(cellfun(@numel,{grid.surf.edgeind}));
    [vertInSurf]=FindVertInSurf(grid);
    cellFaceMap(2:numel(vertInSurf)+1)=cell([1,size(vertInSurf)]);
    for ii=2:numel(vertInSurf)+1
        cellFaceMap{ii}=int2str(vertInSurf(ii-1).vertind);
    end
    
    
    [voluOnRight]=FindRightVolu3D(grid);
    voluOnLeft=abs(voluOnRight-3);
    voluSurfInd=vertcat(grid.surf.voluind)';
    cellFaceMap{end+1}=int2str(voluSurfInd(sub2ind(size(voluSurfInd),...
        voluOnLeft,1:size(voluSurfInd,2))));
    cellFaceMap{end+1}=int2str(voluSurfInd(sub2ind(size(voluSurfInd),...
        voluOnRight,1:size(voluSurfInd,2))));
    
end
