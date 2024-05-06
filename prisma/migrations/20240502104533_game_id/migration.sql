/*
  Warnings:

  - You are about to drop the column `game` on the `Game` table. All the data in the column will be lost.
  - Added the required column `gameId` to the `Game` table without a default value. This is not possible if the table is not empty.

*/
-- AlterTable
ALTER TABLE "Game" DROP COLUMN "game",
ADD COLUMN     "gameId" INTEGER NOT NULL;
