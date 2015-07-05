

-- red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
-- blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
-- green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
-- white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
-- yellow = gr.material({1.0, 1.0, 0}, {0.1, 0.1, 0.1}, 10)
-- orange = gr.material({1.0, .5, 0}, {0.1, 0.1, 0.1}, 10)
-- purple =  gr.material({1.0, 0, 1.0}, {0.1, 0.1, 0.1}, 10)
-- black =  gr.material({0, 0, 0}, {0.1, 0.1, 0.1}, 10)
-- -- Create root nodes.
-- root = gr.node('root')
-- root:translate(0,50,0);
-- root:scale(.5,.5,.5);
-- jointPoint = gr.sphere('JointPoint');
-- 			jointPoint:scale(.1,.1,30);

-- 	--torso
-- 	torso = gr.node('torso')
-- 	root:add_child(torso)
-- 	torsoG = gr.sphere('torsoG');
-- 	torso:add_child(torsoG)
-- 		--shoulders
-- 		shoulders = gr.node('shoulders')
-- 		torso:add_child(shoulders)
-- 		shouldersG = gr.sphere('shouldersG')
-- 		shoulders:add_child(shouldersG);
-- 		shoulders:translate(0,85,0);
-- 		shouldersG:scale(2,.3,.3)
-- 			--neck
-- 			neck = gr.joint('neck', {-50, 0, 50}, {0, 0, 0})
-- 			shoulders:add_child(neck)
-- 			neckG = gr.sphere('neckG')
-- 			neck:add_child(neckG);
-- 			-- neck:add_child(jointPoint)
-- 			neck:translate(0,20,0);
-- 			neckG:scale(0.1,.2,.2)
-- 				--head
-- 				head = gr.joint('head', {-50, 0, 50}, {-50, 0, 50})
-- 				neck:add_child(head)
-- 				headG = gr.sphere('headG')
-- 				head:add_child(headG);
-- 				-- head:add_child(jointPoint)
-- 				head:translate(0,10,0);
-- 				headG:translate(0,30,0)
-- 				headG:scale(.4,.4,.4)
-- 					-- --nose
-- 					noseG = gr.sphere('noseG')
-- 					head:add_child(noseG);
-- 					noseG:translate(0,30,30);
-- 					noseG:scale(.05,.05,.05)
-- 			--leftUpperArm
-- 			leftUpperArm = gr.joint('leftUpperArm', {-180, 0, 0}, {0, 0, 90})
-- 			-- leftUpperArm:add_child(jointPoint)
-- 			shoulders:add_child(leftUpperArm)
-- 			leftUpperArmG = gr.sphere('leftUpperArmG')
-- 			leftUpperArm:add_child(leftUpperArmG);
-- 			leftUpperArm:translate(120,0,0);
-- 			leftUpperArmG:translate(0,-40,0)
-- 			leftUpperArmG:scale(0.1,.5,.2)
-- 			leftUpperArm:rotate('x',-90)

-- 				--leftLowerArm
-- 				leftLowerArm = gr.joint('leftLowerArm', {-150, 0, 0}, {0, 0, 0})
-- 				leftUpperArm:add_child(leftLowerArm)
-- 				-- leftLowerArm:add_child(jointPoint)
-- 				leftLowerArmG = gr.sphere('leftLowerArmG')
-- 				leftLowerArm:add_child(leftLowerArmG);
-- 				leftLowerArm:translate(0,-80,0);
-- 				leftLowerArmG:translate(0,-30,0);
-- 				leftLowerArmG:scale(0.1,.5,.2)
-- 				leftLowerArm:rotate('x',-100)
-- 					--leftHand
-- 					leftHand = gr.joint('leftHand', {-20, 0, 20}, {-20, 0, 20})
-- 					leftLowerArm:add_child(leftHand)
-- 					-- leftHand:add_child(jointPoint)
-- 					leftHandG = gr.sphere('leftHandG')
-- 					leftHand:add_child(leftHandG)
-- 					leftHand:translate(0,-60,0)
-- 					leftHandG:translate(0,-20,0)
-- 					leftHandG:scale(0.1,.3,.2)
-- 			--rightUpperArm
-- 			rightUpperArm = gr.joint('rightUpperArm', {-180, 0, 0}, {-90, 0, 0})
-- 			-- rightUpperArm:add_child(jointPoint)
-- 			shoulders:add_child(rightUpperArm)
-- 			rightUpperArmG = gr.sphere('rightUpperArmG')
-- 			rightUpperArm:add_child(rightUpperArmG);
-- 			rightUpperArm:translate(-120,0,0);
-- 			rightUpperArmG:translate(0,-40,0)
-- 			rightUpperArmG:scale(0.1,.5,.2)

-- 				--rightLowerArm
-- 				rightLowerArm = gr.joint('rightLowerArm', {-150, 0, 0}, {0, 0, 0})
-- 				rightUpperArm:add_child(rightLowerArm)
-- 				-- rightLowerArm:add_child(jointPoint)
-- 				rightLowerArmG = gr.sphere('rightLowerArmG')
-- 				rightLowerArm:add_child(rightLowerArmG);
-- 				rightLowerArm:translate(0,-80,0);
-- 				rightLowerArmG:translate(0,-30,0);
-- 				rightLowerArmG:scale(0.1,.5,.2)
-- 					--rightHand
-- 					rightHand = gr.joint('rightHand', {-20, 0, 20}, {-20, 0, 20})
-- 					rightLowerArm:add_child(rightHand)
-- 					-- rightHand:add_child(jointPoint)
-- 					rightHandG = gr.sphere('rightHandG')
-- 					rightHand:add_child(rightHandG)
-- 					rightHand:translate(0,-60,0)
-- 					rightHandG:translate(0,-20,0)
-- 					rightHandG:scale(0.1,.3,.2)
-- 		--hips
-- 		hips = gr.node('hips')
-- 		torso:add_child(hips)
-- 		hipsG = gr.sphere('hipsG')
-- 		hips:add_child(hipsG);
-- 		hips:translate(0,-85,0);
-- 		hipsG:scale(1.5,.3,.3)
-- 		--leftUpperLeg
-- 			leftUpperLeg = gr.joint('leftUpperLeg', {-180, 0, 0}, {0, 0, 90})
-- 			hips:add_child(leftUpperLeg)
-- 			-- leftUpperLeg:add_child(jointPoint)
-- 			leftUpperLegG = gr.sphere('leftUpperLegG')
-- 			leftUpperLeg:add_child(leftUpperLegG);
-- 			leftUpperLeg:translate(100,0,0);
-- 			leftUpperLegG:translate(0,-40,0);
-- 			leftUpperLegG:scale(0.1,.5,.2)
-- 				--leftLowerLeg
-- 				leftLowerLeg = gr.joint('leftLowerLeg', {0, 0, 50}, {0, 0, 0})
-- 				leftUpperLeg:add_child(leftLowerLeg)
-- 				-- leftLowerLeg:add_child(jointPoint)
-- 				leftLowerLegG = gr.sphere('leftLowerLegG')
-- 				leftLowerLeg:add_child(leftLowerLegG);
-- 				leftLowerLeg:translate(0,-80,0);
-- 				leftLowerLegG:translate(0,-30,0);
-- 				leftLowerLegG:scale(0.1,.5,.2)
-- 				-- leftLowerArm:rotate('x',-50)
-- 					--leftFoot
-- 					leftFoot = gr.joint('leftFoot', {-20, 0, 20}, {0, 0, 0})
-- 					leftLowerLeg:add_child(leftFoot)
-- 					-- leftFoot:add_child(jointPoint)
-- 					leftFootG = gr.sphere('leftFootG')
-- 					leftFoot:add_child(leftFootG)
-- 					leftFoot:translate(0,-60,0)
-- 					leftFootG:translate(0,-10,5);
-- 					leftFootG:scale(.1,.1,.3)
-- 		--rightUpperLeg
-- 			rightUpperLeg = gr.joint('rightUpperLeg', {-180, 0, 0}, {-90, 0, 0})
-- 			hips:add_child(rightUpperLeg)
-- 			-- rightUpperLeg:add_child(jointPoint)
-- 			rightUpperLegG = gr.sphere('rightUpperLegG')
-- 			rightUpperLeg:add_child(rightUpperLegG);
-- 			rightUpperLeg:translate(-100,0,0);
-- 			rightUpperLegG:translate(0,-40,0);
-- 			rightUpperLegG:scale(0.1,.5,.2)
-- 				--rightLowerLeg
-- 				rightLowerLeg = gr.joint('rightLowerLeg', {0, 0, 50}, {0, 0, 0})
-- 				rightUpperLeg:add_child(rightLowerLeg)
-- 				-- rightLowerLeg:add_child(jointPoint)
-- 				rightLowerLegG = gr.sphere('rightLowerLegG')
-- 				rightLowerLeg:add_child(rightLowerLegG);
-- 				rightLowerLeg:translate(0,-80,0);
-- 				rightLowerLegG:translate(0,-30,0);
-- 				rightLowerLegG:scale(0.1,.5,.2)
-- 					--rightFoot
-- 					rightFoot = gr.joint('rightFoot', {-20, 0, 20}, {0, 0, 0})
-- 					rightLowerLeg:add_child(rightFoot)
-- 					-- rightFoot:add_child(jointPoint)
-- 					rightFootG = gr.sphere('rightFootG')
-- 					rightFoot:add_child(rightFootG)
-- 					rightFoot:translate(0,-60,0)
-- 					rightFootG:translate(0,-10,5);
-- 					rightFootG:scale(.1,.1,.3)

-- --Set colors
-- headG:set_material(white)
-- neckG:set_material(green)
-- leftUpperArmG:set_material(green)
-- leftLowerArmG:set_material(red)
-- leftHandG:set_material(yellow)
-- rightUpperArmG:set_material(green)
-- rightLowerArmG:set_material(red)
-- rightHandG:set_material(yellow)
-- leftUpperLegG:set_material(green)
-- leftLowerLegG:set_material(red)
-- leftFootG:set_material(yellow)
-- rightUpperLegG:set_material(green)
-- rightLowerLegG:set_material(red)
-- rightFootG:set_material(yellow)
-- torsoG:set_material(blue)
-- shouldersG:set_material(yellow)
-- hipsG:set_material(yellow)
-- noseG:set_material(red)
-- jointPoint:set_material(black)
-- return root




red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
yellow = gr.material({1.0, 1.0, 0}, {0.1, 0.1, 0.1}, 10)
orange = gr.material({1, 0.2, 0}, {0.1, 0.1, 0.1}, 10)
purple =  gr.material({1.0, 0, 1.0}, {0.1, 0.1, 0.1}, 10)
black =  gr.material({0, 0, 0}, {0.1, 0.1, 0.1}, 10)
-- Create root nodes.
root = gr.node('root')

sun = gr.node('sun')
sunG = gr.sphere('sunG')
sun:scale(1.5,1.5,1.5)


root:add_child(sun)
sun:add_child(sunG)

sunG:set_material(orange)
return root