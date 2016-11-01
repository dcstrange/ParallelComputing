-module(ring).
-compile(export_all).

listen(MyID,NextPID)->
        receive
        {message,FromID,Countdown}->
        if 
		Countdown > 0 ->
                        io:format("process ~w receives message from ~w~n",[MyID,FromID]),
                        NextPID!{message,MyID,Countdown-1};
		true -> {donothing}	
        end,
        listen(MyID,NextPID)
        end.

creatprocess(MyID,MaxID,FirstPID,M)->
	Next=
	fun()->
	if 
	MyID==10 -> FirstPID;
	MyID<10 -> spawn(fun()->creatprocess(MyID+1,MaxID,FirstPID,M) end)
	end
	end,
	NextPID=Next(),
	io:format("process ~w generate a new message~n",[MyID]),
	NextPID!{message,MyID,M-1},
	listen(MyID,NextPID).

start(M,N,_) ->	io:format("~w~n",[self()]),creatprocess(1,N,self(),M).

