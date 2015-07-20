//================================================LOAD GLOBALS==================================================//
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
var scene = new THREE.Scene();
var myTrees = [];
var cubeAnimationProperties = {
	moving: false,
	dir: 0,
	moveSpeed: 1,
	progress: 0,
	curCol: 0
}
var gameSpeed =1;

var particleSystem;
var particleCount;
var particleSystemActivate = false;


var context;
var sourceNode;
var splitter;
var analyser;
var analyser2;
var frequencyData;
var baseTree;
var moveSound;
var dodgeSound;
var gameOverSound;
var powerUpSound;


var rowAlt = 0;
var gameOver = false;




var cubeAnimationTimer;

var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);



//================================================LOAD WORLD==================================================//

loadCamera();
loadSun();
loadLight();
loadFloor(20, 95);
loadCube();
loadBaseTree();
loadSounds();
loadParticleSystem();


var treeAnimationTimer = window.setInterval(function() {
	moveTrees(.1);
	if (!cubeAnimationProperties.moving) {
		detectCollision();
	}
}, 5);

var treeGenTimer = window.setInterval(function() {
	genTrees();
}, 1000/gameSpeed);


//================================================ANIMATE===================================================//
var animate = function() {
	if (gameOver) {
		return;
	}

	updateParticles();
	requestAnimationFrame(animate);

	analyser.getByteFrequencyData(frequencyData);

	camera.position.y = cameraOriginalY;
	camera.position.y += getVolumeAvgNormalized(frequencyData);
	renderer.render(scene, camera);

};

animate();

//================================================FUNCTIONS==================================================//


function startCubeAnimationTimer() {
	var moveSpeed = 2 * gameSpeed;
	var moveDistance = (floor.width / grid.numCol) / (100/moveSpeed);
	cubeAnimationTimer = setInterval(function() {
		if (cubeAnimationProperties.dir == 1) {
			cube.position.x += moveDistance;
		} else if (cubeAnimationProperties.dir == -1) {
			cube.position.x -= moveDistance;
		}
		cubeAnimationProperties.progress += moveSpeed;
		// console.log(cubeAnimationProperties.progress);
		if (cubeAnimationProperties.progress >= 100) {
			stopCubeAnimationTimer();
		}
		detectCollision();
	}, 1);

}

function stopCubeAnimationTimer() {
	clearInterval(cubeAnimationTimer);
	cubeAnimationProperties.moving = false;
}

function moveCube(dir) {
	if (cubeAnimationProperties.moving) {
		return;
	}
	if ((dir == 1 && cubeAnimationProperties.curCol == grid.numCol - 1) || (dir == -1 && cubeAnimationProperties.curCol == 0)) {
		return;
	}
	cubeAnimationProperties.moving = true;
	cubeAnimationProperties.dir = dir;
	cubeAnimationProperties.curCol += dir;
	cubeAnimationProperties.progress = 0;
	moveSound.play();
	startCubeAnimationTimer();
}

function removeTree(i) {
	scene.remove(myTrees[i].top);
	// console.log(myTrees[i].bot);
	if (myTrees[i].bot != 0) {
		// console.log("removed tree");
		scene.remove(myTrees[i].bot);
	}
	else {
		particleSystemActivate = false;
		// console.log("removed particle system");
	}
	var removeTree = myTrees.splice(i, 1);
	removeTree = null;
}

function detectCollision() {
	if(myTrees.length <= 0 ) return;
	var gridDistance = (floor.width / grid.numCol);
	if (Math.abs(myTrees[0].top.position.z - cube.position.z) >= 1.5) {
		return;
	}
	var rowCheck = myTrees[0].row;
	var currentRow = rowCheck;
	var i = 0;
	while (i < myTrees.length && myTrees[i].row == rowCheck) {

		var tree = myTrees[i].top;
		cubex = cube.position.x;
		treex = tree.position.x;
		distance = Math.abs(cubex - treex);
		if (distance < 1.75) {
			// console.log("Distance Between objects " + Math.abs(cubex - treex) + ", index i: " + i);
			if (myTrees[i].bot != 0) {
				killGame();
			}
			else{
				powerUpSound.play();
				gameSpeed += .1;

				clearInterval(treeGenTimer);
				treeGenTimer = window.setInterval(function() {
					genTrees();
				}, 1000/(gameSpeed));

			}
			return;
		}
		if (distance < 7.2) {
			dodgeSound.play();
			//console.log("near miss, distance :" + distance + ", grid Distance :" + 2 * gridDistance);
		}

		removeTree(i);

		i++;
	}
}

function killGame() {
	gameOver = true;
	clearInterval(treeGenTimer);
	clearInterval(treeAnimationTimer);
	clearInterval(cubeAnimationTimer);
	sourceNode.stop();
	gameOverSound.play();


}


function moveTrees(d) {

	size = myTrees.length;
	// console.log(size);

	for (index = 0; index < size; index++) {
		value = myTrees[index];
		moveTree(value, d);
		if (value.top.position.z > 1) {
			removeTree(index);
			index--;
			size--;
		}
	}
}

function genTrees() {
	var locs = genRandomArray(0, grid.numCol - 1, 2);
	for (i = 0; i < locs.length; i++) {
		myTrees.push(makeTree(0, locs[i]));

	}

	rowAlt = (rowAlt + 1) % 2;

}


function genRandomArray(min, max, n) {
	var arry = [];
	do {
		var num = Math.floor(Math.random() * (max + 1 - min)) + min;
		if (arry.indexOf(num) == -1) {
			arry.push(num);
		}
	} while (arry.length != n);

	return arry;
}


function makeTree(row, col) {

	var randPower = Math.floor(Math.random() * 6) + 1;
	row = row + 1;
	var offset = 2;
	if (randPower == 1 && !particleSystemActivate) {
		// console.log ("making upgrade");
		particleSystem.position.z = -floor.adjustedHeight + ((floor.adjustedHeight / grid.numRow) * row) + offset;
		particleSystem.position.x = -floor.adjustedWidth / 2 + ((floor.adjustedWidth / grid.numCol) * col) + offset;

		scene.add(particleSystem);
		particleSystemActivate = true;
		return (newTree = {
			top: particleSystem,
			bot: 0,
			row:rowAlt
		});

	} else {
		cubeTop = baseTree.top.clone();
		cubeBot = baseTree.bot.clone();

		cubeTop.position.z = -floor.adjustedHeight + ((floor.adjustedHeight / grid.numRow) * row) + offset;
		cubeBot.position.z = -floor.adjustedHeight + ((floor.adjustedHeight / grid.numRow) * row) + offset;
		cubeTop.position.x = -floor.adjustedWidth / 2 + ((floor.adjustedWidth / grid.numCol) * col) + offset;
		cubeBot.position.x = -floor.adjustedWidth / 2 + ((floor.adjustedWidth / grid.numCol) * col) + offset;

		scene.add(cubeTop);
		scene.add(cubeBot);

		return (newTree = {
			top: cubeTop,
			bot: cubeBot,
			row: rowAlt
		});
	}

}

function moveTree(tree, z) {
	z = z * gameSpeed;
	tree.top.position.z += z;
	// console.log(particleSystem.position.z);
	if (tree.bot != 0) {
		tree.bot.position.z += z;
	} 

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
	var geometry = new THREE.BoxGeometry(1.5, 1.5, 1.5);

	cube = new THREE.Mesh(geometry, material);
	loadBaseTree
	cube.overdraw = true;
	cube.castShadow = true;
	var offset = 2;
	var row = grid.numRow;
	var col = Math.floor(grid.numCol / 2);
	cube.position.z = -floor.adjustedHeight + ((floor.adjustedHeight / grid.numRow) * row) + offset - .25;
	cube.position.x = -floor.adjustedWidth / 2 + ((floor.adjustedWidth / grid.numCol) * col) + offset;
	cube.position.y += .25;

	cubeAnimationProperties.curCol = col;
	scene.add(cube);

}

function loadCamera() {
	camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 70);
	farPlane = 70;
	camera.position.z = 10;
	camera.position.x = 0;
	camera.position.y = 6;
	camera.rotation.x = toRad(-20);

	cameraOriginalY = camera.position.y;

}

function loadSounds() {

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

	moveSound = new Audio('content/move.ogg');
	dodgeSound = new Audio('content/dodge.ogg');
	gameOverSound = new Audio('content/boom.mp3');
	powerUpSound = new Audio('content/powerup.mp3');
}


function loadParticleSystem() {

	particleCount = 10000,
	particles = new THREE.Geometry(),
	pMaterial = new THREE.ParticleBasicMaterial({
		color: 0xEB1B1B,
		size: .1,
		// map: THREE.ImageUtils.loadTexture(
		// 	"textures/particle.png"
		// 	),
	blending: THREE.AdditiveBlending,
	transparent: true
});

	var range = 1.5;
	// now create the individual particles
	for (var p = 0; p < particleCount; p++) {

		// create a particle with random
		// position values, -250 -> 250
		var particle = new THREE.Vector3(
			Math.random() * range - range / 2,
			Math.random() * range - range / 2,
			Math.random() * range - range / 2
			);

		// add it to the geometry
		particles.vertices.push(particle);
	}

	// create the particle system
	particleSystem = new THREE.ParticleSystem(
		particles,
		pMaterial);

	particleSystem.sortParticles = true;

	// add it to the scene
	// scene.add(particleSystem);


}

function updateParticles() {

	// add some rotation to the system
	// particleSystem.rotation.x += 0.01;

	var pCount = particleCount;
	while (pCount--) {
		// get the particle
		var particle = particles.vertices[pCount];

		if (Math.abs(particle.y) > particleSystem.position.y + Math.random() * 8) {
			particle.y = particleSystem.position.y;
		}
		// if (Math.abs(particle.x - particleSystem.position.x) > Math.random() * 0.5) {
		// 	particle.x = particleSystem.position.x;
		// }

		particle.y += Math.random() * .05;
		// particle.x += Math.random() * .1 * (Math.random() < 0.5 ? -1 : 1);
		particleSystem.geometry.verticesNeedUpdate = true;

	}
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
	return 4 * sum / (buff.length * 100);
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

document.onkeydown = function(e) {
	switch (e.keyCode) {
		case 37:
		moveCube(-1);
		break;
		case 39:
		moveCube(1);
		break;

	}
}