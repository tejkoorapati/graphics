-- A simple test scene featuring some spherical pandas grazing
-- around Stonehenge.  "Assume that pandas are spheres..."

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {.7, 0.7, 0.7}, 5)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
mat1 = gr.material({0.7, 0.7, 0.7}, {1, 1, 1}, 100)
pond = gr.material({0, 0, 0.7}, {1, 1, 1}, 100)
black = gr.material({0, 0, 0.0}, {0.3, 0.3, 0.3}, 20)
white = gr.material({1, 1, 1}, {0.3, 0.3, 0.3}, 20)


inst = gr.node('inst')

jail = gr.node('jail')
inst:add_child(jail)
jail:translate(0, 0, -8)

p1 = gr.nh_box('p1', {0, 0, 0}, 1)
jail:add_child(p1)
p1:set_material(mat1)
p1:translate(-2.4, 0, -0.4)
p1:scale(0.8, 4, 0.8)

p2 = gr.nh_box('p2', {0, 0, 0}, 1)
jail:add_child(p2)
p2:set_material(mat1)
p2:translate(1.6, 0, -0.4)
p2:scale(0.8, 4, 0.8)


-- #############################################
-- Let's assume that pandas are spheres
-- #############################################

panda = gr.node('the_panda')

for _, spec in pairs({
	{{'body', {0, 0, 0}, 1.0}, white},
	{{'head', {.9, .3, 0}, 0.6}, white},
	{{'rEar', {1.1, .8, -.3}, 0.3}, black},
	{{'lEar', {1.1, .8, .3}, 0.3}, black},
	{{'tail', {-.94, .34, 0}, 0.2}, black},
	{{'lfleg', {.7, -.7, -.7}, 0.3}, black},
	{{'lrleg', {-.7, -.7, -.7}, 0.3}, black},
	{{'rfleg', {.7, -.7, .7}, 0.3}, black},
	{{'rrleg', {-.7, -.7, .7}, 0.3}, black}
	}) do
part = gr.nh_sphere(unpack(spec[1]))
part:set_material(spec[2])
panda:add_child(part)
end


-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 20)
scene:translate(0,0,1)


plane = gr.mesh('plane', {
	{ -1, 0, -1 }, 
	{  1, 0, -1 }, 
	{  1, 0, 1 }, 
	{ -1, 0, 1  }
	}, {
	{3, 2, 1, 0}
	})
scene:add_child(plane)
plane:set_material(grass)
plane:scale(10, 10, 10)

pondNode = gr.nh_sphere('pond', {0, 0, 0}, 1)
scene:add_child(pondNode)
pondNode:set_material(pond)
pondNode:scale(3, 0.1, 3)


panda1 = gr.node('panda1')
panda1:add_child(panda)
panda1:translate(-6,1,0)
panda1:scale(1.4,1.4,1.4)

panda2 = gr.node('panda2')
panda2:add_child(panda)
panda2:translate(6,1,0)
panda2:scale(1.4,1.4,1.4)
panda2:rotate("Y",180)

panda3 = gr.node('panda3')
panda3:add_child(panda)
panda3:translate(0,1,6)
panda3:scale(1.4,1.4,1.4)
panda3:rotate("Y",90)

pandaBoss = gr.node('pandaBoss')
pandaBoss:add_child(panda)
pandaBoss:translate(0,1,-6)
pandaBoss:scale(4,4,4)
pandaBoss:rotate("Y",270)

pandaKid = gr.node('pandaKid')
pandaKid:add_child(panda)
pandaKid:translate(0,6,-6)
pandaKid:scale(1,1,1)
pandaKid:rotate("Y",270)
pandaKid:rotate("X",45)


scene:add_child(panda1)
scene:add_child(panda2)
scene:add_child(panda3)
scene:add_child(pandaBoss)
scene:add_child(pandaKid)


for i = 1, 6 do
	an_jail = gr.node('jail' .. tostring(i))
	an_jail:scale(1,6,1)
	an_jail:rotate('Y', (i-1) * 60)
	scene:add_child(an_jail)
	an_jail:add_child(jail)
end

gr.render(scene,
	'sample.png', 500, 500,
	{0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	{0.4, 0.4, 0.4}, {
	gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}),
	gr.light({-200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}),


	})
