%%% ======================================================================
%%% @author Gao Ya'nan <abutter.gao@gmail.com>
%%% @doc Functions for enumlating operations between 4 number to get the
%%%      given result 
%%% @copyright 2013 Gao Ya'nan
%%% @version 0.1
%%% ======================================================================

-module(enumop).

%% API
-export([guess_op/5]).

guess_op(A, B, C, D, Result) ->
	List = [[Op, Nr] || Op <- enum_op(), Nr <- enum_nr(A, B, C, D)],
	lists:filter(make_filter(Result), lists:map(fun eval_node/1, List)).

%%
%% Put 4 numbers to its position.
%%
enum_nr(A, B, C, D) ->
	List = [A, B, C, D],
	[{W, X, Y, Z} || W <- List,
					 X <- List -- [W],
					 Y <- List -- [W, X],
					 Z <- List -- [W, X, Y]].

%%
%% Select 3 from 6 operators.
%%
enum_op() ->
	List = [iadd, isub, imul, idiv, rsub, rdiv],
	[{Opa, Opb, Opc} || Opa <- List, Opb <- List, Opc <- List].

%%
%% There are two different forms : Opc(Opa,Opb) and Opc(Opb(Opa)).
%%
eval_node(Node) ->
	try
		[{Opa, Opb, Opc}, {W, X, Y, Z}] = Node,
		Ra = eval(Opc, eval(Opb, eval(Opa, W, X), Y), Z),
		Rb = eval(Opc, eval(Opa, W, X), eval(Opb, Y, Z)),
		[{Ra, Rb} | Node]
	catch
		% Do not panic when meeting error:badarith
		error:badarith ->
			[{nil, nil} | Node]
	end.

%%
%% Work out the value by opcode and nubmers.
%%
eval(iadd, X, Y) -> X + Y;
eval(isub, X, Y) -> X - Y;
eval(rsub, X, Y) -> Y - X;
eval(imul, X, Y) -> X * Y;
eval(idiv, X, Y) -> X / Y;
eval(rdiv, X, Y) -> Y / X.

%%
%% Make a filter for the right value.
%%
make_filter(Value) ->
	fun ([{Ra, Rb}, _, _])->
		(is_integer(Ra) andalso (Ra == Value)) orelse
		(is_integer(Rb) andalso (Rb == Value))
	end.
