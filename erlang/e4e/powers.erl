%%% ======================================================================
%%% @author Gao Ya'nan <abutter.gao@gmail.com>
%%% @doc Functions to work out the value of X^N
%%% @copyright 2013 Gao Ya'nan
%%% @version 0.1
%%% ======================================================================

-module (powers).

%% API
-export ([raise/2]).

%% -----------------------------------------------------------------------
%% @doc 
%% -----------------------------------------------------------------------

-spec(raise(number(),number()) -> number()).

raise(_, 0) -> 1;
raise(M, 1) -> M;
raise(M, N) when N > 0 -> M * raise(M, N - 1);
raise(M, N) when N < 0 -> 1.0 / raise(M, -N).
