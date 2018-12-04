#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "pawn.hh"
#include "igameboard.hh"

#include <map>
#include <unordered_map>


namespace Student {

class GameBoard : public Common::IGameBoard
{
public:
    /**
      * @default Default constructor.
      */
    GameBoard() = default;

    /**
      * @brief Default virtual destructor.
      */
    virtual ~GameBoard() = default;

    /**
     * @brief checkTileOccupation Checks the current amount of pawns on the tile
     * @param tileCoord The location of the tile in coordinates.
     * @return The number of the pawns in the tile or -1 if the tile does not exist.
     * @post Exception quarantee: strong
     */
    virtual int checkTileOccupation(Common::CubeCoordinate tileCoord) const;

    /**
     * @brief isWaterTile checks if the tile is a water tile.
     * @param tileCoord The location of the tile in coordinates.
     * @return true, if the tile is a water tile, else (or if the tile does not exist) false.
     * @post Exception quarantee: nothrow
     */
    virtual bool isWaterTile(Common::CubeCoordinate tileCoord) const;


    /**
     * @brief getHex returns the hex gameboard tile
     * @param hexCoord The location of the hex in coordinates.
     * @return Shared pointer to the hex or nullptr, if the hex not exists.
     * @post Exception quarantee: nothrow
     */
    virtual std::shared_ptr<Common::Hex> getHex(
            Common::CubeCoordinate hexCoord) const;

    /**
     * @brief addPawn adds a new pawn to the game
     * @param playerId. Id of player, whose pawn is added
     * @param pawnId. Id of the pawn
     * @post Pawn is added to the game. Exception quarantee: basic
     */
    virtual void addPawn(int playerId, int pawnId);

    /**
     * @brief addPawn adds a new pawn to the game
     * @param playerId. Id of player, whose pawn is added
     * @param pawnId. Id of the pawn
     * @param coord. CubeCoordinate, where pawn is added
     * @post Pawn is added to the game. Exception quarantee: basic
     */
    virtual void addPawn(int playerId, int pawnId, Common::CubeCoordinate coord);

    /**
     * @brief movePawn sets a new location for the pawn.
     * @param pawnId The identifier of the pawn.
     * @param pawnCoord The target location of the pawn in coordinates.
     * @pre Pawn exists
     * @post Pawn is moved to the target location. Exception quarantee: basic
     */
    virtual void movePawn(int pawnId, Common::CubeCoordinate pawnCoord);

    /**
     * @brief removePawn removes a pawn.
     * @param pawnId The identifier of the pawn.
     * @pre Pawn exists
     * @post pawn matching the id is removed. Exception quarantee: basic
     */
    virtual void removePawn(int pawnId);

    /**
     * @brief addActor adds a new actor to the game board
     * @param actor
     * @param actorCoord
     * @pre coordinates must contain a hex
     * @post actor has been added to the hex in target coordinates
     */
    virtual void addActor(std::shared_ptr<Common::Actor> actor,
                          Common::CubeCoordinate actorCoord);

    /**
     * @brief moveActor sets a new location for the actor.
     * @param actorId The identifier of the actor.
     * @param actorCoord The target location of the actor in coordinates.
     * @pre Actor exists
     * @post actor actorId is moved to a new location: Exception quarantee: basic
     */
    virtual void moveActor(int actorId, Common::CubeCoordinate actorCoord);

    /**
     * @brief removeActor removes an actor.
     * @param actorId The identifier of the actor.
     * @pre Actor exists
     * @post Actor actorId is removed. Exception quarantee: basic
     */
    virtual void removeActor(int actorId);

    /**
     * @brief addTransport adds a new transport to the game board
     * @param transport transport to be added
     * @param coord
     * @pre coordinates must contain a hex
     * @post Transport has been added to the hex in target coordinates
     */
    virtual void addTransport(std::shared_ptr<Common::Transport> transport,
                              Common::CubeCoordinate coord);

    /**
     * @brief moveTransport sets a new location for the transport.
     * @param id The identifier of the transport.
     * @param coord The target location of the transport in coordinates.
     * @post transport is moved to a new location: Exception quarantee: basic
     */
    virtual void moveTransport(int id, Common::CubeCoordinate coord);

    /**
     * @brief removeTransport removes an transport.
     * @param id The identifier of the transport.
     * @post transport removed from the gameboard. Exception quarantee: basic
     */
    virtual void removeTransport(int id);

    /**
     * @brief addHex adds a new hex tile to the board
     * @param newHex Pointer of a new hex to add
     * @pre newHex is valid
     * @post newHex is added to the board. Any existing hex at the same
     * coordinates is replaced. Exception quarantee: basic
     */
    virtual void addHex(std::shared_ptr<Common::Hex> newHex);

    /**
     * @brief returnHexes, returns the _hexes map
     * @return _hexes map
     */
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> returnHexes();

    /**
     * @brief checkIfActorExists checks if any actors or Transports of a given
     * type are on the board.
     * @param type, the type of the actor or Transport to search for.
     * @return bool:true, if actor or transport exists, false otherwise.
     */
    bool checkIfActorOrTransportExists(std::string type);

    /**
     * @brief getActor returns an actor.
     * @param actorID The id of the actor that is to be returned.
     * @pre actor exists
     * @return std::shared_ptr<Common::Actor>, pointer to
     * the actor indicated by the paramer actorID.
     */
    std::shared_ptr<Common::Actor> getActor(int actorID);

    /**
     * @brief getActor returns an transport.
     * @param actorID The id of the transport that is to be returned.
     * @pre transport exists
     * @return std::shared_ptr<Common::Transport>, pointer to
     * the transport indicated by the paramer transportID.
     */
    std::shared_ptr<Common::Transport> getTransport(int transportID);

    /**
     * @brief getPawnsLeft returns a pawn ptr from given pawnId
     * @return the pawn
     */
    std::shared_ptr<Common::Pawn> getPawn(int pawnId);

    /**
     * @brief getWinner returns the winning player's Id if there is only one pawn
     * left in the game, otherwise returns 0.
     * @return The winning player's Id if there is one, 0 otherwise.
     */
    int getWinner();

    /**
     * @brief pawnsLeft - return the amount of pawns left
     * @return pawns left
     */
    unsigned int pawnsLeft();


private:
    /**
     * @brief Data structures required for storing the GameEngine's logical
     * hexes, pawns, actors and transports.
     */
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> _pawns;
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> _hexes;
    std::map<int, std::shared_ptr<Common::Actor>> _actors;
    std::map<int, std::shared_ptr<Common::Transport>> _transports;

};

}

#endif // GAMEBOARD_HH
