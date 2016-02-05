-- include files.
include( "utils.lua" );
include( "events.lua" );
include( "base.lua" );
include( "concommands.lua" );


function __LostConnection( )

	print( "Lost connection." );

end

function __ConsoleCommand( data )
	local cmd , args = unpack( data:explode( " " , 2 ) );
	print( "cmd: " .. cmd )
	print( "args: " .. tostring( args ) )
	ProcessConsoleCommand( cmd:lower( ) , args );
end

function __RawData( data )

	print( data );
	
	local lines = data:explode( "\n" );
	
	for _ , line in pairs( lines ) do
		Log( line );
		ProcessRaw( line );
	end
end

function Send( data )
	print( "\n>>> " .. data .. "\n\n\n" );
	__Send( data .. "\r\n" );
end
