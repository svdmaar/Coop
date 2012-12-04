struct STriangleData ( 
	vertexCoor1, vertexCoor2, vertexCoor3, 
	texCoor1, texCoor2, texCoor3, 
	color1, color2, color3, 
	normalVector,
	bitmapFileName
)

function getTriangleData objectData triangleIndex  =
(
	out = STriangleData();
	
	clipu = 0.0;
	clipv = 0.0;
	clipw = 0.0;
	cliph = 0.0;

	out.bitmapFileName = "";
	
	-- basic data
	meshObject = objectData.mesh;
	triangle = meshObject.faces[triangleIndex];
	vertices = meshObject.vertices;
	
	-- indeces in arrays
	triangleVertexIndeces = getFace meshObject triangleIndex;
	triangleTVertexIndeces = getTVFace meshObject triangleIndex;
	triangleMatID = getFaceMatID meshObject triangleIndex;
	
	-- regular vertex coordinates
	out.vertexCoor1 = vertices[triangleVertexIndeces[3]].pos + objectData.pos;
	out.vertexCoor2 = vertices[triangleVertexIndeces[2]].pos + objectData.pos;
	out.vertexCoor3 = vertices[triangleVertexIndeces[1]].pos + objectData.pos;

	if(objectData.material != undefined) then
	(
		-- texture clipping
		clipu = objectData.material[triangleMatID].diffuseMap.clipu;
		clipv = objectData.material[triangleMatID].diffuseMap.clipv;
		clipw = objectData.material[triangleMatID].diffuseMap.clipw;
		cliph = objectData.material[triangleMatID].diffuseMap.cliph;
		
		-- bitmap filename
		out.bitmapFileName = objectData.material[triangleMatID].diffuseMap.fileName;
	)
		
	-- textures
	matTexCoor1 = getTVert meshObject triangleTVertexIndeces[3];
	out.texCoor1 = [clipu + matTexCoor1.x * clipw, clipv + ( 1.0 - matTexCoor1.y) * cliph, 0.0];
	matTexCoor2 = getTVert meshObject triangleTVertexIndeces[2];
	out.texCoor2 = [clipu + matTexCoor2.x * clipw, clipv + ( 1.0 - matTexCoor2.y) * cliph, 0.0];
	matTexCoor3 = getTVert meshObject triangleTVertexIndeces[1];
	out.texCoor3 = [clipu + matTexCoor3.x * clipw, clipv + ( 1.0 - matTexCoor3.y) * cliph, 0.0];

	-- colors
	colorVec = [objectData.wirecolor.r, objectData.wirecolor.g, objectData.wirecolor.b]
	out.color1 = colorVec;
	out.color2 = colorVec;
	out.color3 = colorVec;
		
	-- normal
	out.normalVector = getFaceNormal meshObject triangleIndex;
		
	return out
)

function getFileNameIndex fileName fileNameArray =
(
	index = findItem fileNameArray fileName;
	
	if(index == 0) then
	(
		append fileNameArray fileName;
	)
	
	index = findItem fileNameArray fileName;
	return index;
)

function writeHeaderToFile fileHandle =
(
	format "eshfile 1\n" to:fileHandle;
	format "%\n" objects.count to:fileHandle;
	
	triangleCount = 0;
	for i = 1 to objects.count do
	(
		triangleCount = triangleCount + objects[i].mesh.faces.count;
	)
	
	format "%\n" triangleCount to:fileHandle;
)

function writeTriangleDataToFile triangleData bitmapArray fileHandle =
(
	-- vertex coordinates
	format "% % %\n" triangleData.vertexCoor1.x triangleData.vertexCoor1.y triangleData.vertexCoor1.z to:fileHandle;
	format "% % %\n" triangleData.vertexCoor2.x triangleData.vertexCoor2.y triangleData.vertexCoor2.z to:fileHandle;
	format "% % %\n" triangleData.vertexCoor3.x triangleData.vertexCoor3.y triangleData.vertexCoor3.z to:fileHandle;
	
	-- normal vector
	format "% % %\n" triangleData.normalVector.x triangleData.normalVector.y triangleData.normalVector.z to:fileHandle;

	-- colors
	format "% % %\n" triangleData.color1.x triangleData.color1.y triangleData.color1.z to:fileHandle;
	format "% % %\n" triangleData.color2.x triangleData.color2.y triangleData.color2.z to:fileHandle;
	format "% % %\n" triangleData.color3.x triangleData.color3.y triangleData.color3.z to:fileHandle;

	-- texture coordinates
	format "% %\n" triangleData.texCoor1.x triangleData.texCoor1.y to:fileHandle;
	format "% %\n" triangleData.texCoor2.x triangleData.texCoor2.y to:fileHandle;
	format "% %\n" triangleData.texCoor3.x triangleData.texCoor3.y to:fileHandle;
	
	if(triangleData.bitmapFileName != "") then
	(
		-- add bitmap file name to array and store index
		bitmapIndex = (getFileNameIndex triangleData.bitmapFileName bitmapArray) - 1;
		format "%\n" bitmapIndex to:fileHandle;
	)
	else
	(
		format "-1\n" to:fileHandle;
	)
)

function writeObjectToFile objectIndex bitmapArray fileHandle =
(
	format "%\n" objects[objectIndex].name to:fileHandle;
	format "%\n" objects[objectIndex].mesh.faces.count to:fileHandle;
	
	if(objects[objectIndex].material != undefined) then
	(
		format "1\n" to:fileHandle;
	)
	else
	(
		format "0\n" to:fileHandle;
	)
	
	for i = 1 to objects[objectIndex].mesh.faces.count do
	(
		triangleData = getTriangleData objects[objectIndex] i;
		writeTriangleDataToFile triangleData bitmapArray fileHandle;
	)
)

function writeBitmapToFile fileName fileHandle =
(
	readBitmap = openBitmap(fileName);
	
	format "%\n" fileName to:fileHandle;
	format "% %\n" readBitmap.width readBitmap.height to:fileHandle;
	
	for rowIndex = 0 to (readBitmap.height - 1) do
	(
		readRowData = getPixels readBitmap [0, rowIndex] readBitmap.width;

		for columnIndex = 1 to readBitmap.width do
		(
			redValue = readRowData[columnIndex].r as integer;
			greenValue = readRowData[columnIndex].g as integer;
			blueValue = readRowData[columnIndex].b as integer;

			-- always write full alpha
			format "256 % % % " redValue greenValue blueValue to:fileHandle;
		)
		
		format "\n" to:fileHandle;
	)
)

function writeObjectsToFile fileHandle =
(
	bitmapArray = #();
	
	-- write objects to file
	for i = 1 to objects.count do
	(
		writeObjectToFile i bitmapArray fileHandle;
	)
	
	-- write bitmaps to file
	format "% \n" bitmapArray.count to:fileHandle;
	
	for i = 1 to bitmapArray.count do
	(
		writeBitmapToFile bitmapArray[i] fileHandle;
	)
)

macroScript ExportToEsh category:"FileOutput" tooltip:"Export to esh-File"
(
	if(maxFileName == "") then
	(
		messageBox "Please save the scene first";
	)
	else
	(
		baseFileName = getFileNameFile maxFileName;
		outputFileName = "D:\\temp\\esh\\" + baseFileName + ".esh";
		
		outFile = openFile outputFileName mode:"wt";
		
		writeHeaderToFile outFile;
		writeObjectsToFile outFile;
		
		close outFile;
	)
)

-- blah123 = getTriangleData objects[1].mesh  9;

