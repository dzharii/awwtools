-- Function to calculate Fibonacci number in pure Lua
function fibonacci(n)
    if n <= 1 then
        return n
    end

    local a, b = 0, 1
    for i = 2, n do
        local c = a + b
        a = b
        b = c
    end
    return b
end

-- Define the number to calculate Fibonacci for
local n = 10
print("Calculating Fibonacci of " .. n)

-- Call the function and print the result
local result = fibonacci(n)
print("Fibonacci(" .. n .. ") = " .. result)
