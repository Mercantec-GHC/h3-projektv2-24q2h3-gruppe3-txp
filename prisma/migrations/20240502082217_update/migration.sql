/*
  Warnings:

  - You are about to drop the column `game` on the `Highscore` table. All the data in the column will be lost.

*/
-- AlterTable
CREATE SEQUENCE highscore_replayid_seq;
ALTER TABLE "Highscore" DROP COLUMN "game",
ALTER COLUMN "replayId" SET DEFAULT nextval('highscore_replayid_seq');
ALTER SEQUENCE highscore_replayid_seq OWNED BY "Highscore"."replayId";
