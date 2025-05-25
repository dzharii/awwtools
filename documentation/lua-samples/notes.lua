--- Retrieve user input by concatenating all arguments except the first.
-- If no additional arguments are provided, prints an error and returns nil.
-- @return string|nil concatenated arguments or nil if none provided
local function get_input()
    if #arg <= 1 then
        io.stderr:write("Error: notes are empty\n")
        return nil
    end
    local input = table.concat(arg, " ", 2)
    print(("Input received: %s"):format(input))
    return input
end

--- Get current date and time in ISO format.
-- @return string date and time as "YYYY-MM-DD HH-mm"
local function get_timestamp()
    local ts = os.date("%Y-%m-%d %H-%M")
    print(("Timestamp generated: %s"):format(ts))
    return ts
end

--- Prepend a new entry to notes.md or create the file if it does not exist.
-- Entry format
--   YYYY-MM-DD HH-mm
--   <user input>
local function write_entry()
    local input = get_input()
    if not input then
        return
    end

    local timestamp = get_timestamp()
    local entry = ("%s\n%s\n\n"):format(timestamp, input)
    local path = "notes.md"
    local existing = ""
    local file = io.open(path, "r")

    if file then
        existing = file:read("*a")
        file:close()
        print("notes.md exists; prepending new entry.")
    else
        print("notes.md does not exist; creating new file.")
    end

    file = io.open(path, "w")
    if not file then
        io.stderr:write(("Error: could not open %s for writing\n"):format(path))
        return
    end

    file:write(entry, existing)
    file:close()

    print("Operation successful. Content written:")
    print(entry)
end

write_entry()
