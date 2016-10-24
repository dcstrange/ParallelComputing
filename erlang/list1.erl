-module(list1).
-compile([export_all]).
	
getBig(X,Y) when X>Y -> X;
getBig(X,Y) when X=<Y -> Y.
getSmall(X,Y) when X>Y -> Y;
getSmall(X,Y) when X=<Y -> X.

maxl([])->ERR;
maxl([H|T])->maxl(T,H);
maxl([],CurMax)->CurMax;
maxl([H|T],CurMax)->maxl(T,getBig(H,CurMax)).

minl([])->ERR;
minl([H|T])->minl(T,H);
minl([],CurMin)->CurMin;
minl([H|T],CurMin)->minl(T,getSmall(H,CurMin)).

%returns a tuple containing the min and max of the list L.
min_max([],{CurMin,CurMax})->{CurMin,CurMax};
min_max([H|T],{CurMin,CurMax})->min_max(T,{getSmall(CurMin,H),getBig(CurMax,H)}).

%function sum/1 which given a positve integer N, will return the sum of the all integers between 1 and N.
sum(1)->1;
sum(N) when N>1 ->N+sum(N-1);
sum(_)-> "Error: param(1) requires a positive integer.".

%function sum/2 which given two positve integers N and M where N=<M, will return the sum of the all integers between N and M.
sum(N,N)->N;
sum(N,M) when N<M ->M+sum(N,M-1);
sum(_,_)-> "Error: param(1) requires equal with or smaller than param(2).".

%creating list
appendlist(X,MaxNum) when X>=MaxNum -> [MaxNum];
appendlist(X,MaxNum)-> [X|appendlist(X+1,MaxNum)].
create(N) when N<1 ->[];
create(N)->
	appendlist(1,N).

%creating reverse-list
reverse_create(M) when M<1 ->[];
reverse_create(1)->[1];
reverse_create(N)->[N|reverse_create(N-1)].

