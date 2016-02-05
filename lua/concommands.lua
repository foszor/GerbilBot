local ConsoleCommands = { };

function ProcessConsoleCommand( cmd , args )
	if ( not ConsoleCommands[ cmd ] ) then
		print( "Unknown command." );
		return;
	end
	
	if ( args ) then
		args = args:explode( " " );
	else
		args = { };
	end
	
--	local success = pcall( ConsoleCommands[ cmd ] , args );
	ConsoleCommands[ cmd ]( args );
end


function AddConsoleCommand( name , func )
	ConsoleCommands[ name ] = func;
	
end


local function reload( args )
	include( "wheel.lua" );
	print( "*** scripts reloaded ***" );
end
AddConsoleCommand( "reload" , reload )


local function raw( args )
	local text = table.concat( args , " " );
	Send( text );
end
AddConsoleCommand( "raw" , raw )


local function connect( args )
	-- connect
	__Connect( "irc.gamesurge.net", 6667 );
	Send( "USER ircbot . . :GerbilBot!" );
	SetNick( "GerbilBot" );
end
AddConsoleCommand( "connect" , connect )


local function runlua( args )
	local text = table.concat( args , " " );
	local results = { pcall( loadstring( text ) ) };
	PrintTable( results );
end
AddConsoleCommand( "lua" , runlua )


local function setnick( args )
	SetNick( args[ 1 ] );
end
AddConsoleCommand( "nick" , setnick )


local function say( args )
	local chan = args[ 1 ];
	table.remove( args , 1 );
	local text = table.concat( args , " " );
	Message( chan , text );
end
AddConsoleCommand( "say" , say )
