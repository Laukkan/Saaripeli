 #ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <map>
#include <unordered_map>
#include "pawn.hh"

#include "igameboard.hh"

class GameBoard : public Common::IGameBoard
{
public:
    GameBoard();
    ~GameBoard();

    /**
     * @brief checkTileOccupation Checks the current amount of pawns on the tile
     * @param tileCoord The location of the tile in coordinates.
     * @return The number of the pawns in the tile or -1 if the tile does not exist.
     * @post Exception quarantee: strong
     */
    int checkTileOccupation(Common::CubeCoordinate tileCoord) const;

    /**
     * @brief isWaterTile checks if the tile is a water tile.
     * @param tileCoord The location of the tile in coordinates.
     * @return true, if the tile is a water tile, else (or if the tile does not exist) false.
     * @post Exception quarantee: nothrow
     */
    bool isWaterTile(Common::CubeCoordinate tileCoord) const;


    /**
     * @brief getHex returns the hex gameboard tile
     * @param hexCoord The location of the hex in coordinates.
     * @return Shared pointer to the hex or nullptr, if the hex not exists.
     * @post Exception quarantee: nothrow
     */
    std::shared_ptr<Common::Hex> getHex(Common::CubeCoordinate hexCoord) const;

    /**
     * @brief addPawn adds a new pawn to the game
     * @param playerId. Id of player, whose pawn is added
     * @param pawnId. Id of the pawn
     * @post Pawn is added to the game. Exception quarantee: basic
     */
    void addPawn(int playerId, int pawnId);

    /**
     * @brief movePawn sets a new location for the pawn.
     * @param pawnId The identifier of the pawn.
     * @param pawnCoord The target location of the pawn in coordinates.
     * @pre Pawn exists
     * @post Pawn is moved to the target location. Exception quarantee: basic
     */
    void movePawn(int pawnId, Common::CubeCoordinate pawnCoord);

    /**
     * @brief removePawn removes a pawn.
     * @param pawnId The identifier of the pawn.
     * @pre Pawn exists
     * @post pawn matching the id is removed. Exception quarantee: basic
     */
    void removePawn(int pawnId);

    /**
     * @brief moveActor sets a new location for the actor.
     * @param actorId The identifier of the actor.
     * @param actorCoord The target location of the actor in coordinates.
     * @pre Actor exists
     * @post actor actorId is moved to a new location: Exception quarantee: basic
     */
    void moveActor(int actorId, Common::CubeCoordinate actorCoord);

    /**
     * @brief removeActor removes an actor.
     * @param actorId The identifier of the actor.
     * @pre Actor exists
     * @post Actor actorId is removed. Exception quarantee: basic
     */
    void removeActor(int actorId);

    /**
     * @brief addHex adds a new hex tile to the board
     * @param newHex Pointer of a new hex to add
     * @pre newHex is valid
     * @post newHex is added to the board. Any existing hex at the same
     * coordinates is replaced. Exception quarantee: basic
     */
    void addHex(std::shared_ptr<Common::Hex> newHex);

private:

    std::unordered_map<int,std::shared_ptr<Common::Pawn>> _pawns;
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> _hexes;
    std::map<int, Common::CubeCoordinate> _actors;

};
#endif // GAMEBOARD_HH
