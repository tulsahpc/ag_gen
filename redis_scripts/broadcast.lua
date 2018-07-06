local count = 0

local broadcast = redis.call("lrange", KEYS[1], 0, -1)

for _, key in ipairs(broadcast) do
    redis.call("incr", key)
    count = count + 1
end
return count
