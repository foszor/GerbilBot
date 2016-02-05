
-- write a file.
function WriteFile( filename , data )
	-- open file.
	local f = io.open( filename , "w" );
	
	-- check success.
	if( not f ) then return; end 
	
	-- write the data.
	f:write( data );
	
	-- close file.
	f:close( );
	
end


-- read a file.
function ReadFile( filename )
	-- open file.
	local f = io.open( filename, "r" );
	
	-- check success.
	if( not f ) then return; end
	
	-- read data.
	local s = f:read( "*all" );
	
	-- close file.
	f:close( );
	
	-- return data.
	return s;
	
end


-- append file.
function AppendFile( filename , data )
	-- read file.
	local old = ReadFile( filename ) or "";
	
	-- write old + new data.
	WriteFile( filename , old .. data );
	
end


-- explode string.
function string:explode( sep , limit )
	-- default limit.
	limit = limit or 0;
	
	-- idiot fix.
	if ( limit == 1 ) then
		return { self };
	end
	
	-- results table.
	local tbl = { };
	
	-- position.
	local ll = 0;
	
	-- infinite loop. (OF DOOM!)
	while ( true ) do
		-- find position.
		local l = self:find( sep , ll , true );
		
		-- check.
		if ( l ~= nil ) then
			-- add result.
			tbl[ #tbl + 1 ] = self:sub( ll , l - 1 );
			ll = l + 1;
		else
			-- remainder and break.
			tbl[ #tbl + 1 ] = self:sub( ll );
			break;
		end
		
		-- check limit.
		if ( limit ~= 0 and #tbl >= limit - 1 ) then
			-- remainder and break.
			tbl[ #tbl + 1 ] = self:sub( ll );
			break;
		end
	end
	
	-- return results.
	return tbl;
	
end


-- clean string.
function string:clean( )
	return ( self:gsub( "^%s*(.-)%s*$" , "%1" ) );
	
end


-- dump table content.
function PrintTable( tbl , indent , done )
	-- history.
	done = done or { };
	
	-- indent amount.
	indent = indent or 0;
	
	-- loop table.
	local key , value;
	for key , value in pairs( tbl ) do
	
		-- check sub-table.
		if ( type( value ) == "table" and not done[ value ] ) then
			-- go recursive.
			done[ value ] = true;
			print( string.rep( "\t" , indent ) .. tostring( key ) .. ":" );
			
			-- call self.
			PrintTable( value , indent + 2 , done );
		else
			-- print.
			print( string.rep( "\t" , indent ) .. tostring( key ) .. "\t=\t" .. tostring( value ) );
		end
	end
	
end


-- log text.
function Log( text )
	-- append to log.
	AppendFile( "log.txt" , text .. "\n" );
	
end
