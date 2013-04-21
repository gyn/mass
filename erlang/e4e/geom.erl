%%% ======================================================================
%%% @author Gao Ya'nan <abutter.gao@gmail.com>
%%% @doc Functions for calculating areas of geometric shapes.
%%% @copyright 2013 Gao Ya'nan
%%% @version 0.1
%%% ======================================================================

-module (geom).

%% API
-export ([area/2]).

%% -----------------------------------------------------------------------
%% @doc Calculates the area of a rectangle, given the length and width.
%%      Returns the product of its arguments.
%% -----------------------------------------------------------------------

-spec(area(number(),number()) -> number()).

area(Length, Width) -> Length * Width.