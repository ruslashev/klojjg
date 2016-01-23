planets = {
	{ position = { x = 400, y = 400 }, velocity = { vx = 0, vy = 0 },
			mass = 42500, radius = 100, color = { 250, 175, 18 }},
	{ position = { x = 550, y = 400 }, velocity = { vx = 0, vy = 16 },
			mass = 60, radius = 10, color = { 140, 50, 30 }},
	{ position = { x = 700, y = 400 }, velocity = { vx = -1, vy = 9 },
			mass = 10, radius = 3, color = { 18, 180, 230 }},
	{ position = { x = 694, y = 400 }, velocity = { vx = 0, vy = 7.7 },
			mass = 0.3, radius = 1, color = { 240, 240, 240 }},
}

local zoom = 1
local timescale = 1

function love.load()
	love.window.setMode(800, 800)
end

function love.update(dt)
	local scaled_dt = dt * timescale
	local G = 6.674e-1
	for i = 1, #planets do
		local force_x = 0
		local force_y = 0
		local x = planets[i].position.x
		local y = planets[i].position.y
		local mass = planets[i].mass
		for j = 1, #planets do
			if i ~= j then
				local other_x = planets[j].position.x
				local other_y = planets[j].position.y
				local other_mass = planets[j].mass
				local distance = math.sqrt(math.pow(x - other_x, 2)
					+ math.pow(y - other_y, 2))
				local x_inc = (other_x - x) / distance
				local y_inc = (other_y - y) / distance
				force_x = force_x + G * mass * other_mass / math.pow(distance, 2) * x_inc
				force_y = force_y + G * mass * other_mass / math.pow(distance, 2) * y_inc
			end
		end
		local acceleration_x = force_x / mass
		local acceleration_y = force_y / mass
		planets[i].velocity.vx = planets[i].velocity.vx + acceleration_x * scaled_dt;
		planets[i].velocity.vy = planets[i].velocity.vy + acceleration_y * scaled_dt;
		planets[i].position.x = planets[i].position.x + planets[i].velocity.vx * scaled_dt;
		planets[i].position.y = planets[i].position.y + planets[i].velocity.vy * scaled_dt;
	end
end

function love.keyreleased(key)
	if key == "a" then
		timescale = timescale + 1
	elseif key == "z" then
		timescale = timescale - 1
	end
end

function love.draw()
	love.graphics.print("similarity to solar system: not even close", 10, 10)
	love.graphics.print(string.format("timescale: %g ; a/z to change", timescale), 10, 30)
	for _,v in ipairs(planets) do
		love.graphics.setColor(v.color)
		love.graphics.circle("fill", v.position.x, v.position.y, v.radius*zoom, v.radius*3)
	end
end

