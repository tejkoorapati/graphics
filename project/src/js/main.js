//========================LOAD GLOBALS==========================//
var PI = 3.14159265359;

var grid = {
	numRow: 7,
	numCol: 5,
	rows: [],
	cols: []
}

var camera;
var farPlane;
var cameraOriginalY;
var cube;
var floor;
var sun;
var lensFlare;
var context;
var sourceNode;
var splitter;
var analyser;
var analyser2;
var frequencyData;
var baseTree;
var scene = new THREE.Scene();


var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);



//========================LOAD WORLD==========================//

loadCamera();
loadSun();
loadLight();
loadFloor(20, 95);
loadCube();
loadBaseTree();
loadMusic();

var myTree = makeTree(6, 0);
var myTree = makeTree(6, 1);
var myTree = makeTree(6, 2);
var myTree = makeTree(6, 3);
var myTree = makeTree(6, 4);
var myTree = makeTree(5, 0);
var myTree = makeTree(4, 0);
var myTree = makeTree(3, 0);
var myTree = makeTree(2, 0);
var myTree = makeTree(1, 0);
var myTree = makeTree(0, 0);

for(i = 0; i < 7; i ++){
	for (j = 0; j < 5; j++){
		makeTree(i,j);
	}
}


//========================ANIMATE==========================//
var animate = function() {
	requestAnimationFrame(animate);

	cube.rotation.y += toRad(1);
	analyser.getByteFrequencyData(frequencyData);

	camera.position.y = cameraOriginalY;
	camera.position.y += getVolumeAvgNormalized(frequencyData);

	renderer.render(scene, camera);
	// moveTree(myTree,-0.1)


};


animate();



//========================FUNCTIONS==========================//



function makeTree(row, col) {

	cubeTop = baseTree.top.clone();
	cubeBot = baseTree.bot.clone();
	row = row + 1;

	var offset = 2;
	cubeTop.position.z = -floor.adjustedHeight + ((floor.adjustedHeight / grid.numRow) * row) + offset;
	cubeBot.position.z = -floor.adjustedHeight + ((floor.adjustedHeight / grid.numRow) * row) + offset;
	cubeTop.position.x = -floor.adjustedWidth / 2 + ((floor.adjustedWidth / grid.numCol) * col) + offset;
	cubeBot.position.x = -floor.adjustedWidth / 2 + ((floor.adjustedWidth / grid.numCol) * col) + offset;

	scene.add(cubeTop);
	scene.add(cubeBot);

	return (newTree = {
		top: cubeTop,
		bot: cubeBot
	});
}

function moveTree(tree, z) {
	tree.top.position.z += z;
	tree.bot.position.z += z;

}


function loadBaseTree() {

	var textureTop = THREE.ImageUtils.loadTexture('textures/treeTop.jpg');
	var textureBot = THREE.ImageUtils.loadTexture('textures/treeBot.jpg');

	var materialTop = new THREE.MeshLambertMaterial();
	materialTop.map = textureTop;
	var geometryTop = new THREE.BoxGeometry(2, 1.5, 1.5);

	var cubeTop = new THREE.Mesh(geometryTop, materialTop);
	cubeTop.overdraw = true;
	cubeTop.castShadow = true;


	var materialBot = new THREE.MeshLambertMaterial();
	materialBot.map = textureBot;
	var geometryBot = new THREE.BoxGeometry(.5, 2, .5);

	var cubeBot = new THREE.Mesh(geometryBot, materialBot);
	cubeBot.overdraw = true;
	cubeBot.castShadow = true;

	cubeTop.position.y += 1;

	baseTree = {
		top: cubeTop,
		bot: cubeBot
	}
}


function loadSun() {
	sun = new THREE.SpotLight(0x444444);
	sun.shadowCameraNear = 0.1;
	sun.position.set(-15, 20, -60);
	sun.castShadow = true;
	sun.shadowCameraVisible = true;

	scene.add(sun);
}

function loadLight() {
	var ambient = new THREE.AmbientLight(0x444444);
	scene.add(ambient);

	renderer.shadowMapEnabled = true;
	renderer.shadowMapType = THREE.PCFSoftShadowMap;


	var textureFlare0 = THREE.ImageUtils.loadTexture("textures/lensFlare.png");
	var flareColor = new THREE.Color(0xffaacc);
	lensFlare = new THREE.LensFlare(textureFlare0, 350, 0.0, THREE.AdditiveBlending, flareColor);
	lensFlare.position.set(sun.position.x, sun.position.y, sun.position.z);
	scene.add(lensFlare);

	// var light = new THREE.DirectionalLight(0x4444cc, 2);
	// light.position.set(1, -1, 1).normalize();
	// scene.add(light);
}



function loadFloor(width, length) {

	var texture = THREE.ImageUtils.loadTexture('textures/earth.jpg');

	texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
	texture.repeat.set(10, 30);

	var material = new THREE.MeshBasicMaterial();
	material.map = texture;
	var geometry = new THREE.BoxGeometry(width, .1, length);

	var floorMesh = new THREE.Mesh(geometry, material);
	floorMesh.translate
	floorMesh.overdraw = true;
	floorMesh.position.y = -.55;
	floorMesh.position.z = -45;
	floorMesh.receiveShadow = true;
	floor = {
		width: width,
		height: length,
		adjustedHeight: farPlane - camera.position.z,
		adjustedWidth: width - 2
	}


	scene.add(floorMesh);

}

function loadCube() {

	var texture = THREE.ImageUtils.loadTexture('textures/brick.jpg');

	var material = new THREE.MeshLambertMaterial();
	material.map = texture;
	var geometry = new THREE.BoxGeometry(1, 1, 1);

	cube = new THREE.Mesh(geometry, material);
	cube.overdraw = true;
	cube.castShadow = true;
	var offset = 2;
	var row = grid.numRow;
	var col = Math.floor(grid.numCol / 2);
	cube.position.z = -floor.adjustedHeight + ((floor.adjustedHeight / grid.numRow) * row) + offset;
	cube.position.x = -floor.adjustedWidth / 2 + ((floor.adjustedWidth / grid.numCol) * col) + offset;
	scene.add(cube);

}

function loadCamera() {
	camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 70);
	farPlane= 70;
	camera.position.z = 10;
	camera.position.x = 0;
	camera.position.y = 6;
	camera.rotation.x = toRad(-20);

	cameraOriginalY = camera.position.y;

}

function loadMusic() {

	getSound('content/music.ogg');
	context = new AudioContext();
	sourceNode = context.createBufferSource();
	splitter = context.createChannelSplitter();
	analyser = context.createAnalyser();
	analyser2 = context.createAnalyser();
	analyser.fftSize = 1024;
	analyser2.fftSize = 1024;
	sourceNode.connect(splitter);
	splitter.connect(analyser, 0);
	splitter.connect(analyser2, 1);
	sourceNode.connect(context.destination);
	frequencyData = new Uint8Array(analyser.frequencyBinCount);
}



function getSound(url) {
	var request = new XMLHttpRequest();

	request.open('GET', url, true);
	request.responseType = 'arraybuffer';

	request.onload = function() {
		context.decodeAudioData(request.response, function(buf) {
			playSound(buf);
		});
	}

	request.send();
}

function playSound(buffer) {
	sourceNode.buffer = buffer;
	sourceNode.start(0);
}

function getVolumeAvgNormalized(buff) {
	var sum = 0;
	$.each(buff, function(index, value) {
		sum += value;
	});
	return 1.5 * sum / (buff.length * 100);
}

function toRad(a) {
	return (a * PI / 180);
}

function getWidth(obj) {
	return obj.geometry.toJSON().width
}

function getHeight(obj) {
	return obj.geometry.toJSON().height
}