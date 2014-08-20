%% @doc vec3 can be used to represent 3D points and vectors.
%% ----------------------------------------------------------------------------

-module(vec3).

% API
-export([length/1, length_squared/1, distance/2, distance_squared/2,
	fuzzy_zero/1, add/2, subtract/2, multiply/1, divide/1, equals/2, dot/2,
	normalized/1, rotate/3, angle/2, absolute/1, cross/2, lerp/3, set_max/2,
	set_min/2]).

-record(vec3, {
	x :: float(),
	y :: float(),
	z :: float()
}).

-export_type([vec3/0]).

-type vec3() :: #vec3{}.

% -----------------------------------------------------------------------------
% btVector3 API
% -----------------------------------------------------------------------------

%% @doc Returns the length of the vector.
-spec length(ThisVec :: vec3()) -> float().
length(ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Returns the length of the vector, squared.
-spec length_squared(ThisVec :: vec3()) -> float().
length_squared(ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return the distance between the ends of this and another vector.
%% This is symantically treating the vector like a point.
-spec distance(OtherVec :: vec3(), ThisVec :: vec3()) -> float().
distance(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return the distance between the ends of this and another vector,
%% squared. This is symantically treating the vector like a point.
-spec distance_squared(OtherVec :: vec3(), ThisVec :: vec3()) -> float().
distance_squared(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Returns true if every element in the vector is approximately zero,
%% +/- `SIMD_EPSILON'. This is more useful than comparing to zero due to float
%% precision errors.
-spec fuzzy_zero(ThisVec :: vec3()) -> bool().
fuzzy_zero(ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Add a vector to this one.
-spec add(OtherVec :: vec3(), ThisVec :: vec3()) -> vec3().
add(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Subtract OtherVec from ThisVec.
-spec subtract(OtherVec :: vec3(), ThisVec :: vec3()) -> vec3().
subtract(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Scale the vector.
-spec multiply(Scalar :: float()) -> vec3().
multiply(Scalar) ->
	exit(nif_library_not_loaded).

%% @doc Inversely scale the vector.
-spec divide(Scalar :: float()) -> vec3().
divide(Scalar) ->
	exit(nif_library_not_loaded).

%% @doc Returns true if OtherVec equals ThisVec.
-spec equals(OtherVec :: vec3(), ThisVec :: vec3()) -> bool().
equals(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return the dot product.
-spec dot(OtherVec :: vec3(), ThisVec :: vec3()) -> float().
dot(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return a normalized version of this vector.
-spec normalized(ThisVec :: vec3()) -> vec3().
normalized(ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return a rotated version of this vector.
-spec rotate(Axis :: vec3(), Angle :: float(), ThisVec :: vec3()) -> vec3().
rotate(Axis, Angle, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return the angle between this and another vector.
-spec angle(OtherVec :: vec3(), ThisVec :: vec3()) -> float().
angle(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return the angle between this and another vector.
-spec absolute(ThisVec :: vec3()) -> vec3().
absolute(ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return the cross product between ThisVec and OtherVec.
-spec cross(OtherVec :: vec3(), ThisVec :: vec3()) -> vec3().
cross(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Return the linear interpolation between this and another vector. Ratio
%% is the ratio between ThisVec and OtherVec (i.e. When Ratio is 0, return
%% ThisVec, when Ration is 1 return OtherVec).
-spec lerp(OtherVec :: vec3(), Ratio :: float(), ThisVec :: vec3()) -> vec3().
lerp(OtherVec, Ratio, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Set each element to the max of the values of ThisVec and the values of
%% OtherVec.
-spec set_max(OtherVec :: vec3(), ThisVec :: vec3()) -> vec3().
set_max(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

%% @doc Set each element to the min of the values of ThisVec and the values of
%% OtherVec.
-spec set_min(OtherVec :: vec3(), ThisVec :: vec3()) -> vec3().
set_min(OtherVec, ThisVec) ->
	exit(nif_library_not_loaded).

% -------------------------------------------------------------------------