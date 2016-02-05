local function OnConnected( )
	Send( "JOIN #pgm" );
end
HookEvent( "OnConnected" , OnConnected );


local function OnChannelMessage( chan , nick , hostmask , text )
--	Message( chan , nick .. " said " .. text );
end
HookEvent( "OnChannelMessage" , OnChannelMessage );


local function OnAction( chan , nick , hostmask , text )
--	Message( chan , "dont do your silly '" .. text .. "' actions with me, " .. nick .. "!" );
	Action( chan , text )
end
HookEvent( "OnAction" , OnAction );


local function OnJoin( chan , nick , hostmask )
	Message( chan , "welcome to " .. chan .. " " .. nick .. "!" );
end
HookEvent( "OnJoin" , OnJoin );


local function OnPart( chan , nick , hostmask )
	Message( chan , "bye " .. nick .. " :(" );
end
HookEvent( "OnPart" , OnPart );


local function OnNickChange( old , new , hostmask )
	Message( new , "you changed your name from " .. old .. " to " .. new );
end
HookEvent( "OnNickChange" , OnNickChange );


local function OnKick( chan , who , reason , by , hostmask )
	Message( chan , who .. " was kicked by " .. by .. " for (" .. reason .. ")" );
end
HookEvent( "OnKick" , OnKick );
