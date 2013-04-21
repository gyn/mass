%%% ======================================================================
%%% @author Gao Ya'nan <abutter.gao@gmail.com>
%%% @doc Functions for calculating areas of geometric shapes.
%%% @copyright 2013 Gao Ya'nan
%%% @version 0.1
%%% ======================================================================

-module (geom).

%% API
-export ([area/3]).

%% -----------------------------------------------------------------------
%% @doc Calculates the area of a rectangle, given the length and width.
%%      Returns the product of its arguments.
%% -----------------------------------------------------------------------

-spec(area(atom(), number(),number()) -> number()).

area(rectangle, Length, Width) when Length >= 0, Width >= 0 ->
	Length * Width;
area(triangle, Hight, Width) when Hight >= 0, Width >= 0 ->
	Hight * Width / 2.0;
area(ellipse, Hight, Width) when Hight >= 0, Width >= 0 ->
	math:pi() * Hight * Width;
area(_, _, _) ->
	0.