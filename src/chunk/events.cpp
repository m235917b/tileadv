#include "chunk/events.hpp"

#include <typeindex>

#include "actor/commands.hpp"
#include "actor/components.hpp"
#include "chunk/resources.hpp"
#include "ecs/ecs.hpp"
#include "player/resources.hpp"
#include "utils/csvreader.hpp"

void subscribeChunkBorderEventListener(ECSContext &context) {
  context.getEventBus().subscribe(
      std::type_index(typeid(ChunkBorderEvent)),
      [](ECSContext &context, const std::any &event) {
        const auto border{std::any_cast<ChunkBorderEvent>(event).border};
        const auto &chunk{
            *std::any_cast<Chunk>(context.getResourceManager().getResource(
                std::type_index(typeid(Chunk))))};
        const auto &id{*std::any_cast<PlayerIDResource>(
            context.getResourceManager().getResource(
                std::type_index(typeid(PlayerIDResource))))};
        auto pos{*std::any_cast<Position>(context.getStore().getComponent(
            id.id, std::type_index(typeid(Position))))};

        switch (border) {
        case ChunkBorder::NORTH: {
          if (chunk.up != "null") {
            auto chunkNew{csvToChunk("world/" + chunk.up + ".cnk")};
            chunkNew.id = chunk.up;
            context.getCommandBuffer().upsertResource(std::move(chunkNew));
            context.getCommandBuffer().clearECSStore();
            context.getCommandBuffer().enqueue(
                std::make_any<PopulateChunk>(PopulateChunk{chunk.up}));
            pos.y = chunk.sizeY - 1;
          }
          break;
        }
        case ChunkBorder::WEST: {
          if (chunk.left != "null") {
            auto chunkNew{csvToChunk("world/" + chunk.left + ".cnk")};
            chunkNew.id = chunk.left;
            context.getCommandBuffer().upsertResource(std::move(chunkNew));
            context.getCommandBuffer().clearECSStore();
            context.getCommandBuffer().enqueue(
                std::make_any<PopulateChunk>(PopulateChunk{chunk.left}));
            pos.x = chunk.sizeX - 1;
          }
          break;
        }
        case ChunkBorder::SOUTH: {
          if (chunk.down != "null") {
            auto chunkNew{csvToChunk("world/" + chunk.down + ".cnk")};
            chunkNew.id = chunk.down;
            context.getCommandBuffer().upsertResource(std::move(chunkNew));
            context.getCommandBuffer().clearECSStore();
            context.getCommandBuffer().enqueue(
                std::make_any<PopulateChunk>(PopulateChunk{chunk.down}));
            pos.y = 0;
          }
          break;
        }
        case ChunkBorder::EAST: {
          if (chunk.right != "null") {
            auto chunkNew{csvToChunk("world/" + chunk.right + ".cnk")};
            chunkNew.id = chunk.right;
            context.getCommandBuffer().upsertResource(std::move(chunkNew));
            context.getCommandBuffer().clearECSStore();
            context.getCommandBuffer().enqueue(
                std::make_any<PopulateChunk>(PopulateChunk{chunk.right}));
            pos.x = 0;
          }
          break;
        }
        }

        context.getCommandBuffer().upsertComponent(
            id.id, std::make_any<Position>(std::move(pos)));
      });
}

void initChunkEvents(ECSContext &context) {
  subscribeChunkBorderEventListener(context);
}
