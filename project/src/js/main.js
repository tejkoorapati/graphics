var PI = 3.14159265359;

var grid = {
	numRow: 20,
	numCol: 5,
	rows: [],
	cols: []
}

var camera;
var context = new AudioContext();
var sourceNode = context.createBufferSource();
var splitter = context.createChannelSplitter();
var analyser = context.createAnalyser();
var analyser2 = context.createAnalyser();

var frequencyData = new Uint8Array(analyser.frequencyBinCount);

var scene = new THREE.Scene();


var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

var texture = THREE.ImageUtils.loadTexture('textures/wood.jpg');
// texture.repeat.set( 4, 2 );
// texture.wrapS = texture.wrapT = THREE.RepeatWrapping;


var material = new THREE.MeshBasicMaterial();
material.map = texture;
var geometry = new THREE.BoxGeometry(1, 1, 1);


var cube = new THREE.Mesh(geometry, material);
cube.overdraw = true;
scene.add(cube);


loadCamera();
loadMusic();







var animate = function() {
	requestAnimationFrame(animate);

	cube.rotation.y += toRad(1);
	analyser.getByteFrequencyData(frequencyData);

	cube.position.y = 0;
	cube.position.y += getVolumeAvgNormalized(frequencyData);

	renderer.render(scene, camera);


};


animate();


function loadCamera() {
	camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 70);
	camera.position.z = 5;
	camera.position.x = 0;
	camera.position.y = 1;

}

function loadMusic() {

	getSound('content/music.ogg');
	analyser.fftSize = 1024;
	analyser2.fftSize = 1024;
	sourceNode.connect(splitter);
	splitter.connect(analyser, 0);
	splitter.connect(analyser2, 1);
	sourceNode.connect(context.destination);
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
	return sum / (buff.length * 100);
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