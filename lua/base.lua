local MyNick;

function SetNick( name )
	Send( "NICK " .. name );
	if ( not MyNick ) then
		MyNick = name;
	end
end

function Message( target , text )
	Send( "PRIVMSG " .. target .. " :" .. text );
end

function Action( target , text )
	Send( "PRIVMSG " .. target .. " :ACTION " .. text .. "" );
end

function ProcessRaw( raw )
	local ParseUsermask = function( usermask )
		local nick , hostmask = unpack( usermask:explode( "!" ) );
		
		return nick , hostmask;
	end

	raw = raw:clean( );
	local args = raw:explode( " " , 3 );
	PrintTable( args );
	
	if ( args[ 1 ] == "PING" ) then
		Send( "PONG " .. args[ 2 ] );
	elseif ( args[ 2 ] == "001" ) then
		RaiseEvent( "OnConnected" );
	elseif ( args[ 2 ] == "PRIVMSG" ) then
		local nick , hostmask = ParseUsermask( args[ 1 ]:sub( 2 ) );
		
		local target , text = unpack( args[ 3 ]:explode( " " , 2 ) );
		text = text:sub( 2 );
		
		if ( text:sub( 1 , 7 ) == "ACTION" ) then
			text = text:sub( 9 , #text - 1 );
			RaiseEvent( "OnAction" , target , nick , hostmask , text );
		elseif ( target:sub( 1 , 1 ) == "#" ) then
			RaiseEvent( "OnChannelMessage" , target , nick , hostmask , text );
		else
			RaiseEvent( "OnPrivateMessage" , nick , hostmask , text );
		end
	elseif ( args[ 2 ] == "PART" ) then
		local nick , hostmask = ParseUsermask( args[ 1 ]:sub( 2 ) );
		RaiseEvent( "OnPart" , args[ 3 ] , nick , hostmask );
	elseif ( args[ 2 ] == "JOIN" ) then
		local nick , hostmask = ParseUsermask( args[ 1 ]:sub( 2 ) );
		RaiseEvent( "OnJoin" , args[ 3 ] , nick , hostmask );
	elseif ( args[ 2 ] == "NICK" ) then
		local nick , hostmask = ParseUsermask( args[ 1 ]:sub( 2 ) );
		local new = args[ 3 ]:sub( 2 );
		if ( nick == MyNick ) then
			MyNick = new;
		end
		RaiseEvent( "OnNickChange" , nick , new , hostmask );
	elseif ( args[ 2 ] == "KICK" ) then
		local nick , hostmask = ParseUsermask( args[ 1 ]:sub( 2 ) );
		local chan , who , reason = unpack( args[ 3 ]:explode( " " , 3 ) );
		RaiseEvent( "OnKick" , chan , who , reason:sub( 2 ) , nick , hostmask )
	end
end


include( "example.lua" );
