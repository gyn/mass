%%% ======================================================================
%%% @author Gao Ya'nan <abutter.gao@gmail.com>
%%% @doc Functions to find the GCD of integers M and N
%%% @copyright 2013 Gao Ya'nan
%%% @version 0.1
%%% ======================================================================

-module (dijkstra).

%% API
-export ([gcd/2]).

%% -----------------------------------------------------------------------
%% @doc Find the GCD of integers M and N
%% -----------------------------------------------------------------------

-spec(gcd(number(),number()) -> number()).

gcd(M, M) ->
	M;
gcd(M, N) when M > N ->
	gcd(M - N, N);
gcd(M, N) when M < N ->
	gcd(M, N - M).