%%
%%
%%
-module (enumop).

%%
%%
%%
-export ([guess_op/5]).

%%
%%
%%
-compile([debug_info]).


guess_op(A, B, C, D, Result) ->
	List = [[Op, Nr] || Op <- enum_op(), Nr <- enum_nr(A, B, C, D)],
	Filter = make_filter(Result),
	lists:filter(Filter, lists:map(fun eval_node/1, List)).


%
%
%
enum_nr(A, B, C, D) ->
	List = [A, B, C, D],
	[{W, X, Y, Z} || W <- List,
					 X <- List -- [W],
					 Y <- List -- [W, X],
					 Z <- List -- [W, X, Y]].

%
%
%
enum_op() ->
	List = [iadd, isub, imul, idiv, rsub, rdiv],
	[{Opa, Opb, Opc} || Opa <- List, Opb <- List, Opc <- List].


eval_node(Node) ->
	try 
		[{Opa, Opb, Opc}, {W, X, Y, Z}] = Node,
		Ra = eval(Opc, eval(Opb, eval(Opa, W, X), Y), Z),
		Rb = eval(Opc, eval(Opa, W, X), eval(Opb, Y, Z)),
		[{Ra, Rb} | Node]
	catch
		%
		%
		%
		error:badarith ->
			[{nil, nil} | Node]
	end.

%
%
%
eval(iadd, X, Y) -> X + Y;
eval(isub, X, Y) -> X - Y;
eval(rsub, X, Y) -> Y - X;
eval(imul, X, Y) -> X * Y;
eval(idiv, X, Y) -> X / Y;
eval(rdiv, X, Y) -> Y / X.

%
%
%
make_filter(Value) ->
	fun ([{Ra, Rb}, _, _])->
		(is_integer(Ra) andalso (Ra == Value)) orelse
		(is_integer(Rb) andalso (Rb == Value))
	end.
