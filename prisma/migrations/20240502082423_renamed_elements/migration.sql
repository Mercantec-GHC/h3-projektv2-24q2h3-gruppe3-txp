/*
  Warnings:

  - You are about to drop the `Highscore` table. If the table is not empty, all the data it contains will be lost.

*/
-- DropForeignKey
ALTER TABLE "Highscore" DROP CONSTRAINT "Highscore_gameId_fkey";

-- DropForeignKey
ALTER TABLE "Highscore" DROP CONSTRAINT "Highscore_replayId_fkey";

-- DropForeignKey
ALTER TABLE "Highscore" DROP CONSTRAINT "Highscore_userId_fkey";

-- DropTable
DROP TABLE "Highscore";

-- CreateTable
CREATE TABLE "Score" (
    "id" SERIAL NOT NULL,
    "userId" INTEGER NOT NULL,
    "score" INTEGER NOT NULL,
    "replayId" SERIAL NOT NULL,
    "gameId" INTEGER NOT NULL,

    CONSTRAINT "Score_pkey" PRIMARY KEY ("id")
);

-- AddForeignKey
ALTER TABLE "Score" ADD CONSTRAINT "Score_userId_fkey" FOREIGN KEY ("userId") REFERENCES "User"("id") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Score" ADD CONSTRAINT "Score_replayId_fkey" FOREIGN KEY ("replayId") REFERENCES "Replay"("id") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Score" ADD CONSTRAINT "Score_gameId_fkey" FOREIGN KEY ("gameId") REFERENCES "Game"("id") ON DELETE RESTRICT ON UPDATE CASCADE;
