//================================================LOAD GLOBALS==================================================//
var PI = 3.14159265359;

var grid = {
	numRow: 7,
	numCol: 5,
	rows: [],
	cols: []
}

var loadingComplete = false;
var camera;
var farPlane;
var cameraOriginalY;
var cube;
var floor;
var sun;
var lensFlare;
var scene = new THREE.Scene();
var backgroundScene;
var backgroundCamera;
var finalRender = false;
var waterTexture;
var iceTexture;
var sunDir = 1;
var scoreElement;
var score = 0;

var myTrees = [];
var cubeAnimationProperties = {
	moving: false,
	dir: 0,
	moveSpeed: 1,
	progress: 0,
	curCol: 0
}



var gameSpeed = 1;
var level = 0;
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
var treeAnimationTimer;
var treeGenTimer;
var levelTimer;
var sunTimer;

//================================================LOAD WORLD==================================================//


var progress;
var renderer = new THREE.WebGLRenderer({
	alpha: true,
	antialias: true
});
renderer.setSize(window.innerWidth, window.innerHeight);

$(document).ready(function() {
	// Handler for .ready() called.
	$("#preScreen").width(window.innerWidth);
	$("#preScreen").height(window.innerHeight);

	progress = new ProgressBar.Circle('#preScreen', {
		width: window.innerWidth,
		height: window.innerHeight,
		color: '#FCB03C',
		strokeWidth: 3,
		trailWidth: 1,
		duration: 1500,
		text: {
			value: '0'
		}
	});
	init();

});
var loadSegments = 8;
var totalLoadProgress = 0;

function init() {

	updateProgressBar("Loading World...", totalLoadProgress / loadSegments);
	loadBackground();
	totalLoadProgress++;
	updateProgressBar("Loading World...", totalLoadProgress / loadSegments);
	loadCamera();
	totalLoadProgress++;
	updateProgressBar("Loading World...", totalLoadProgress / loadSegments);
	loadLight();
	totalLoadProgress++;
	updateProgressBar("Loading Objects...", totalLoadProgress / loadSegments);
	loadFloor(20, 95);
	totalLoadProgress++;
	updateProgressBar("Loading Objects...", totalLoadProgress / loadSegments);
	loadCube();
	totalLoadProgress++;
	updateProgressBar("Loading Objects...", totalLoadProgress / loadSegments);
	loadBaseTree();
	totalLoadProgress++;
	updateProgressBar("Loading Particle System...", totalLoadProgress / loadSegments);
	loadParticleSystem();
	totalLoadProgress++;
	updateProgressBar("Loading Some Nice Music...", totalLoadProgress / loadSegments);
	loadSounds();

}

function updateProgressBar(text, value) {
	progress.animate(value);
	progress.setText(text);
}


function initTimers() {

	treeAnimationTimer = window.setInterval(function() {
		moveTrees(.1);
		updateScore();
		if (!cubeAnimationProperties.moving) {
			detectCollision();
		}
	}, 5);

	treeGenTimer = window.setInterval(function() {
		genTrees();
	}, 1000 / gameSpeed);


	levelTimer = window.setInterval(function() {
		increaseLevel();
	}, 1000 * 15);

	sunTimer = window.setInterval(function() {
		moveSun();
	}, 1);

}

//================================================ANIMATE===================================================//
var animate = function() {
	if (!gameOver) {
		detectCollision();
		updateParticles();
		requestAnimationFrame(animate);

		analyser.getByteFrequencyData(frequencyData);

		camera.position.y = cameraOriginalY;
		camera.position.y += getVolumeAvgNormalized(frequencyData);

	}
	renderer.autoClear = false;
	renderer.clear();
	renderer.render(backgroundScene, backgroundCamera);

	renderer.render(scene, camera);



};

// loadStats();
//================================================FUNCTIONS==================================================//

function loadStats() {
	var script = document.createElement('script');
	script.onload = function() {
		var stats = new Stats();
		stats.domElement.style.cssText = 'position:fixed;left:0;top:0;z-index:10000';
		document.body.appendChild(stats.domElement);
		requestAnimationFrame(function loop() {
			stats.update();
			requestAnimationFrame(loop)
		});
	};
	script.src = '//rawgit.com/mrdoob/stats.js/master/build/stats.min.js';
	document.head.appendChild(script);
}


function startCubeAnimationTimer() {
	var moveSpeed = 2 * gameSpeed;
	var moveDistance = (floor.width / grid.numCol) / (100 / moveSpeed);
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


//modified threejs examples documentation
function lensFlareUpdateCallback(object) {
	var f, fl = object.lensFlares.length;
	var flare;
	var vecX = -object.positionScreen.x * 2;
	var vecY = -object.positionScreen.y * 2;
	for (f = 0; f < fl; f++) {
		flare = object.lensFlares[f];
		flare.x = object.positionScreen.x + vecX * flare.distance;
		flare.y = object.positionScreen.y + vecY * flare.distance;
		flare.rotation = 0;
	}
}

function moveSun() {
	if (sun.position.x >= 30) {
		sunDir = -1;
	} else if (sun.position.x <= -30) {
		sunDir = 1;
	}
	sun.position.setX(sun.position.x + sunDir * .01);
	sun.position.setY(-.02 * Math.pow(sun.position.x, 2) + 20)
	lensFlare.position.copy(sun.position);
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
	} else {
		particleSystemActivate = false;
		// console.log("removed particle system");
	}
	var removeTree = myTrees.splice(i, 1);
	removeTree.top = null;
	removeTree.bot = null;
	removeTree = null;
}

function increaseLevel() {
	if (level == 2) return;
	level++;
	if (level == 1) {
		floor.mesh.material.map = waterTexture;
		sourceNode.playbackRate.value += .01;
		gameSpeed += 1;
	}
	if (level == 2) {
		floor.mesh.material.map = iceTexture;
		sourceNode.playbackRate.value += .01;
		gameSpeed += 1;
	}

	clearInterval(treeGenTimer);
	treeGenTimer = window.setInterval(function() {
		genTrees();
	}, 1000 / (gameSpeed));

}

function detectCollision() {
	if (myTrees.length <= 0) return;
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
				myTrees[i].bot.position.z = cube.position.z - 1.5;
				myTrees[i].top.position.z = cube.position.z - 1.5;
				killGame();
			} else {
				powerUpSound.play();
				gameSpeed += .1;

				clearInterval(treeGenTimer);
				treeGenTimer = window.setInterval(function() {
					genTrees();
				}, 1000 / (gameSpeed));

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
	clearInterval(levelTimer);
	gameOverSound.play();
	sourceNode.stop();
	return;


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

	var randPower = Math.floor(Math.random() * 20) + 1;
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
			row: rowAlt
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

function loadBackground() {

	// Load the background texture
	var texture = THREE.ImageUtils.loadTexture('textures/background.jpg');
	texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
	texture.repeat.set(1, 1);
	var backgroundMesh = new THREE.Mesh(
		new THREE.PlaneGeometry(2, 2, 0),
		new THREE.MeshBasicMaterial({
			map: texture
		}));

	backgroundMesh.material.depthTest = false;
	backgroundMesh.material.depthWrite = false;

	// Create your background scene
	backgroundScene = new THREE.Scene();
	backgroundCamera = new THREE.Camera();
	backgroundScene.add(backgroundCamera);
	backgroundScene.add(backgroundMesh);
}

function loadLight() {

	sun = new THREE.SpotLight(0xffffff);
	// sun.color.setHSL( 0.55,0.9, 0.5 );
	sun.shadowCameraNear = 0.1;
	sun.position.set(-15, 20, -60);
	sun.castShadow = true;
	// sun.shadowCameraVisible = true;

	scene.add(sun);

	var ambient = new THREE.AmbientLight(0x444444);
	scene.add(ambient);



	var textureFlare0 = THREE.ImageUtils.loadTexture("textures/lensFlare.png");
	var textureFlare2 = THREE.ImageUtils.loadTexture("textures/lensFlareArtifact1.png");
	var textureFlare3 = THREE.ImageUtils.loadTexture("textures/lensFlareArtifact2.png");
	var flareColor = new THREE.Color(0xffaacc);
	lensFlare = new THREE.LensFlare(textureFlare0, 700, 0.0, THREE.AdditiveBlending, flareColor);
	lensFlare.position.copy(sun.position);

	// lensFlare.add(textureFlare2, 512, 0.0, THREE.AdditiveBlending);
	// lensFlare.add(textureFlare2, 512, 0.0, THREE.AdditiveBlending);
	// lensFlare.add(textureFlare2, 512, 0.0, THREE.AdditiveBlending);
	lensFlare.add(textureFlare3, 60, 0.6, THREE.AdditiveBlending);
	lensFlare.add(textureFlare3, 70, 0.7, THREE.AdditiveBlending);
	lensFlare.add(textureFlare3, 120, 0.9, THREE.AdditiveBlending);
	lensFlare.add(textureFlare3, 70, 1.0, THREE.AdditiveBlending);

	lensFlare.customUpdateCallback = lensFlareUpdateCallback;

	scene.add(lensFlare);

	renderer.shadowMapEnabled = true;
	renderer.shadowMapType = THREE.PCFSoftShadowMap;
}


function loadScore() {
	scoreElement = document.createElement('div');
	scoreElement.style.position = 'absolute';
	//scoreElement.style.zIndex = 1;    // if you still don't see the label, try uncommenting this
	scoreElement.style.width = 100;
	scoreElement.style.height = 100;
	scoreElement.style.color = 'aqua';
	// scoreElement.style.backgroundColor = "blue";
	scoreElement.innerHTML = "hi there!";
	scoreElement.style.top = 0 + 'px';
	scoreElement.style.left = 0 + 'px';
	document.body.appendChild(scoreElement);
}

function loadFloor(width, length) {

	var texture = THREE.ImageUtils.loadTexture('textures/earth.jpg');

	texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
	texture.repeat.set(5, 20);

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
		adjustedWidth: width - 2,
		mesh: floorMesh
	}


	waterTexture = THREE.ImageUtils.loadTexture('textures/water.jpg');
	iceTexture = THREE.ImageUtils.loadTexture('textures/ice.jpg')


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
	sourceNode.loop = true;
	frequencyData = new Uint8Array(analyser.frequencyBinCount);

	moveSound = new Audio('content/move.ogg');
	dodgeSound = new Audio('content/dodge.ogg');
	gameOverSound = new Audio('content/boom.mp3');
	powerUpSound = new Audio('content/powerup.mp3');
	getSound('content/music2.ogg');
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

function updateScore() {
	score += 1 * gameSpeed;
	scoreElement.innerHTML = "Score: " + Math.floor(score + 1 * gameSpeed);
}


function getSound(url) {
	var request = new XMLHttpRequest();
	request.addEventListener("progress", invokeProgress, false);

	request.open('GET', url, true);
	request.responseType = 'arraybuffer';

	request.onload = function() {
		context.decodeAudioData(request.response, function(buf) {
			playSound(buf);
		});
	}

	request.send();
}

function invokeProgress(oEvent) {
	if (oEvent.lengthComputable) {
		var percentComplete = oEvent.loaded / oEvent.total;
		updateProgressBar("Loading Some Nice Music...", (totalLoadProgress + percentComplete) / loadSegments);

		// ...
	} else {
		// Unable to compute progress information since the total size is unknown
	}
}

function playSound(buffer) {
	sourceNode.buffer = buffer;

	updateProgressBar("Complete!", 1);
	$("#preScreen").fadeOut(1500, function() {
		document.body.appendChild(renderer.domElement);
		initTimers();
		sourceNode.start(0);
		animate();
		loadScore();
	});



}

function getVolumeAvgNormalized(buff) {
	var sum = 0;
	$.each(buff, function(index, value) {
		sum += value;
	});
	return 2 * sum / (buff.length * 100);
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