/*****************************************************************************

		Copyright (c) My Company

 Project:  NW
 FileName: NW.PRO
 Purpose: No description
 Written by: Visual Prolog
 Comments:
******************************************************************************/

include "nw.inc"
include "nw.con"
include "hlptopic.con"

global predicates
 	procedure updateNodes(integer, integer) - (i, i) language stdcall 

global domains

	node = node(integer, integer)
	nodes = node*
	edge = edge(node, node)
	way = edge*
	ways = way*
	
	block = road ; base
	command = up ; down ; left ; right
	commands = command*

global database - mapdb
	types(node, block)
	commands(edge, command)
	
global database - botdb
	bot(command)

predicates

	% Save and load db into file
	loadData()
	saveData()
	
	nondeterm addCommand(command)
	
	nondeterm contain(node, nodes)
	
	nondeterm p_node(node)
	
	nondeterm adjacent(node, node)
	
	nondeterm dfs3(node, nodes, way)
	
	nondeterm dfs4(node, node, nodes, way)
	
	nondeterm set_commands(way, commands)
	
	determ run(integer, integer)
	
clauses
	% Save and load db into file
	saveData():-save("botmap.txt", botdb).
	loadData():-consult("map.txt", mapdb).
	
	addCommand(Command):-asserta(bot(Command), botdb).

	
	p_node(Node):-types(Node, _).

	adjacent(Node1, Node2) :-commands(edge(Node1, Node2), _).
	%adjacent(Node1, Node2) :-commands(edge(Node2, Node1), _).
	
	contain(Elem, [Elem|_Tail]).
	contain(Elem, [_Head|Tail]):-
	contain(Elem, Tail).	

		
	dfs3(From, _, [edge(From, To)]):- 
		adjacent(From, To), types(To, base),!.
	
	dfs3(From, VisitedNodes, [edge(From, X)|TailPath]):-
		adjacent(From, X), 
		not(contain(X, VisitedNodes)),
		dfs3(X, [From|VisitedNodes], TailPath), !.
	
	
	
	
	dfs4(From, To, _, [edge(From, To)]):-
		adjacent(From, To).
	dfs4(From, To, VisitedNodes, [edge(From, X)|TailPath]):-
		adjacent(From, X), 
		not(contain(X, VisitedNodes)),
		dfs4(X, To, [From|VisitedNodes], TailPath).
	
	
	
	set_commands([], []).
	set_commands([E|Edge], [C|Commands]):- commands(E, C), set_commands(Edge, Commands), addCommand(C).
	
	
	run(X, Y):-loadData(),  dfs3(node(X, Y),[],Way),set_commands(Way, Commands),saveData(), !.
		
	updateNodes(X, Y):-run(X, Y).
	
goal
	run(0, 0).
  	

