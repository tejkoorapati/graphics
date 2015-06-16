--
-- CS488 -- Introduction to Computer Graphics
-- 
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

-- rootnode = gr.node('root')

-- red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
-- -- blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
-- green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
-- white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- s0 = gr.sphere('s0')
-- rootnode:add_child(s0)
-- s0:set_material(white)

-- s1 = gr.sphere('s1')
-- rootnode:add_child(s1)
-- s1:scale(0.1, 2.0, 0.1)
-- s1:set_material(red)

-- s2 = gr.sphere('s2')
-- rootnode:add_child(s2)
-- s2:translate(2.0, -2.0, 0.0)
-- s2:rotate('z', -90.0)
-- s2:scale(0.1, 2.0, 0.1)
-- -- s2:set_material(blue)

-- s3 = gr.sphere('s3')
-- rootnode:add_child(s3)
-- s3:scale(0.1, 0.1, 2.0)
-- s3:translate(0.0, -20.0, 1.0)
-- s3:set_material(green)

-- rootnode:translate(-0.75, 0.25, -10.0)
-- rootnode:rotate('y', -20.0)

-- return rootnode

-- rootnode = gr.node('root')
-- rootnode:translate(-2.0, 3.0, -10.0)
-- rootnode:scale(0.6, 0.6, 0.6)
-- rootnode:rotate('y', -10)

-- Torso = gr.sphere('Torso')
-- rootnode:add_child(Torso)
-- Torso:translate(0.0, 0.0, -18)
-- Torso:scale(1.5, 2, 0.7)

-- Shoulders = gr.sphere('Shoulders')
-- rootnode:add_child(Shoulders)
-- Shoulders:translate(0.0, 120, -18)
-- Shoulders:scale(1.5, 0.5, 0.7)
-- ShouldersJoint = gr.joint('ShouldersJoint', {-5.0, 0.0, 5.0}, {0.0, 0.0, 0.0})
-- Shoulders:add_child(ShouldersJoint)
-- ShouldersJoint:rotate('x', -45)
-- ShouldersE = gr.node('ShouldersE')
-- rootnode:add_child(ShouldersE)
-- ShouldersE:translate(0.0, 1.5, -18)

-- return rootnode



-- A humanoid puppet.

-- Create root nodes.
root = gr.node('root')
root:translate(0,50,0);
root:scale(.5,.5,.5);

	--torso
	torso = gr.node('torso')
	root:add_child(torso)
	torsoG = gr.sphere('torsoG');
	torso:add_child(torsoG)
	torso:scale(1,1.4,1)
		--shoulders
		shoulders = gr.node('shoulders')
		torso:add_child(shoulders)
		shouldersG = gr.sphere('shouldersG')
		shoulders:add_child(shouldersG);
		shouldersG:translate(0,85,0);
		shouldersG:scale(2,.3,.3)
			--neck
			neck = gr.joint('neck', {-50, 0, 50}, {0, 0, 0})
			shoulders:add_child(neck)
			neckG = gr.sphere('neckG')
			neck:add_child(neckG);
			neckG:translate(0,100,0);
			neckG:scale(0.1,.2,1)
				--head
				head = gr.joint('head', {-50, 0, 50}, {-50, 0, 50})
				neck:add_child(head)
				headG = gr.sphere('headG')
				head:add_child(headG);
				headG:translate(0,140,0);
				headG:scale(.4,.4,1)
					--head
					headG = gr.sphere('headG')
					head:add_child(headG);
					headG:translate(0,140,0);
					headG:scale(.05,.05,.05)
			--leftUpperArm
			leftUpperArm = gr.joint('leftUpperArm', {-180, 0, 0}, {0, 0, 90})
			shoulders:add_child(leftUpperArm)
			leftUpperArmG = gr.sphere('leftUpperArmG')
			leftUpperArm:add_child(leftUpperArmG);
			leftUpperArmG:translate(120,50,0);
			leftUpperArmG:scale(0.1,.5,1)
				--leftLowerArm
				leftLowerArm = gr.joint('leftLowerArm', {-150, 0, 0}, {0, 0, 0})
				shoulders:add_child(leftLowerArm)
				leftLowerArmG = gr.sphere('leftLowerArmG')
				leftLowerArm:add_child(leftLowerArmG);
				leftLowerArmG:translate(120,0,0);
				leftLowerArmG:scale(0.1,.5,1)
					--leftHand
					leftHand = gr.joint('leftHand', {-20, 0, 20}, {-20, 0, 20})
					shoulders:add_child(leftHand)
					leftHandG = gr.sphere('leftHandG')
					leftHand:add_child(leftHandG);
					leftHandG:translate(120,-40,0);
					leftHandG:scale(0.1,.1,1)
			--rightUpperArm
			rightUpperArm = gr.joint('rightUpperArm', {-180, 0, 0}, {-90, 0, 0})
			shoulders:add_child(rightUpperArm)
			rightUpperArmG = gr.sphere('rightUpperArmG')
			rightUpperArm:add_child(rightUpperArmG);
			rightUpperArmG:translate(-120,50,0);
			rightUpperArmG:scale(0.1,.5,1)
				--rightLowerArm
				rightLowerArm = gr.joint('rightLowerArm', {-150, 0, 0}, {0, 0, 0})
				shoulders:add_child(rightLowerArm)
				rightLowerArmG = gr.sphere('rightLowerArmG')
				rightLowerArm:add_child(rightLowerArmG);
				rightLowerArmG:translate(-120,0,0);
				rightLowerArmG:scale(0.1,.5,1)
					--rightHand
					rightHand = gr.joint('rightHand', {-20, 0, 20}, {-20, 0, 20})
					shoulders:add_child(rightHand)
					rightHandG = gr.sphere('rightHandG')
					rightHand:add_child(rightHandG);
					rightHandG:translate(-120,-40,0);
					rightHandG:scale(0.1,.1,1)
		--hips
		hips = gr.node('hips')
		torso:add_child(hips)
		hipsG = gr.sphere('hipsG')
		hips:add_child(hipsG);
		hipsG:translate(0,-85,0);
		hipsG:scale(1.5,.3,.3)
		--leftUpperLeg
			leftUpperLeg = gr.joint('leftUpperLeg', {-180, 0, 0}, {0, 0, 90})
			hips:add_child(leftUpperLeg)
			leftUpperLegG = gr.sphere('leftUpperLegG')
			leftUpperLeg:add_child(leftUpperLegG);
			leftUpperLegG:translate(100,-120,0);
			leftUpperLegG:scale(0.1,.5,1)
				--leftLowerLeg
				leftLowerLeg = gr.joint('leftLowerLeg', {0, 0, 50}, {0, 0, 0})
				hips:add_child(leftLowerLeg)
				leftLowerLegG = gr.sphere('leftLowerLegG')
				leftLowerLeg:add_child(leftLowerLegG);
				leftLowerLegG:translate(100,-170,0);
				leftLowerLegG:scale(0.1,.5,1)
					--leftFoot
					leftFoot = gr.joint('leftFoot', {-20, 0, 20}, {0, 0, 0})
					hips:add_child(leftFoot)
					leftFootG = gr.sphere('leftFootG')
					leftFoot:add_child(leftFootG);
					leftFootG:translate(100,-210,0);
					leftFootG:scale(0.1,.1,1)


			--rightUpperLeg
			rightUpperLeg = gr.joint('rightUpperLeg', {-180, 0, 0}, {0, 0, 90})
			hips:add_child(rightUpperLeg)
			rightUpperLegG = gr.sphere('rightUpperLegG')
			rightUpperLeg:add_child(rightUpperLegG);
			rightUpperLegG:translate(-100,-120,0);
			rightUpperLegG:scale(0.1,.5,1)
				--rightLowerLeg
				rightLowerLeg = gr.joint('rightLowerLeg', {0, 0, 50}, {0, 0, 0})
				hips:add_child(rightLowerLeg)
				rightLowerLegG = gr.sphere('rightLowerLegG')
				rightLowerLeg:add_child(rightLowerLegG);
				rightLowerLegG:translate(-100,-170,0);
				rightLowerLegG:scale(0.1,.5,1)
					--rightFoot
					rightFoot = gr.joint('rightFoot', {-20, 0, 20}, {0, 0, 0})
					hips:add_child(rightFoot)
					rightFootG = gr.sphere('rightFootG')
					rightFoot:add_child(rightFootG);
					rightFootG:translate(-100,-210,0);
					rightFootG:scale(0.1,.1,1)


return root