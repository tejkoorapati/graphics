-- A simple test scene featuring some spherical cows grazing
-- around Stonehenge.  "Assume that cows are spheres..."

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {.7, 0.7, 0.7}, 5)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
mat1 = gr.material({0.7, 0.7, 0.7}, {1, 1, 1}, 100)
pond = gr.material({0, 0, 0.7}, {1, 1, 1}, 100)

-- ##############################################
-- the jailh
-- ##############################################

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
-- Let's assume that cows are spheres
-- #############################################

cow = gr.node('the_cow')

for _, spec in pairs({
	{'body', {0, 0, 0}, 1.0},
	{'head', {.9, .3, 0}, 0.6},
	{'tail', {-.94, .34, 0}, 0.2},
	{'lfleg', {.7, -.7, -.7}, 0.3},
	{'lrleg', {-.7, -.7, -.7}, 0.3},
	{'rfleg', {.7, -.7, .7}, 0.3},
	{'rrleg', {-.7, -.7, .7}, 0.3}
	}) do
part = gr.nh_sphere(unpack(spec))
part:set_material(hide)
cow:add_child(part)
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


cow1 = gr.node('cow1')
cow1:add_child(cow)
cow1:translate(-6,1,0)
cow1:scale(1.4,1.4,1.4)

cow2 = gr.node('cow2')
cow2:add_child(cow)
cow2:translate(6,1,0)
cow2:scale(1.4,1.4,1.4)
cow2:rotate("Y",180)

cow3 = gr.node('cow3')
cow3:add_child(cow)
cow3:translate(0,1,6)
cow3:scale(1.4,1.4,1.4)
cow3:rotate("Y",90)

cowBoss = gr.node('cowBoss')
cowBoss:add_child(cow)
cowBoss:translate(0,1,-6)
cowBoss:scale(4,4,4)
cowBoss:rotate("Y",270)


scene:add_child(cow1)
scene:add_child(cow2)
scene:add_child(cow3)
scene:add_child(cowBoss)

-- Place a ring of jailhes.

for i = 1, 6 do
	an_jail = gr.node('jail' .. tostring(i))
	an_jail:scale(1,6,1)
	an_jail:rotate('Y', (i-1) * 60)
	scene:add_child(an_jail)
	an_jail:add_child(jail)
end

gr.render(scene,
	'cow_zoo.png', 600, 600,
	{0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	{0.4, 0.4, 0.4}, {
	gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}),
	gr.light({-200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}),


	})
