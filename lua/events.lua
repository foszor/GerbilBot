
-- events storage.
local Events = { };


-- call an event.
function RaiseEvent( name , ... )
	-- not hooked.
	if ( not Events[ name ] ) then
		return;
	end
	
	-- find all hooks.
	local func;
	for _ , func in pairs( Events[ name ] ) do
		-- pcall it, pass arguments.
		local success , result = pcall( func , unpack( arg ) );
		
		-- check for errors.
		if ( success ) then
			-- hook chose to override.
			if ( result ) then
				break;
			end
		end
	end
	
end


-- hook an event.
function HookEvent( name , func )
	-- validate table.
	Events[ name ] = Events[ name ] or { };
	
	-- insert into table.
	Events[ name ][ #Events[ name ] + 1 ] = func;
	
end
