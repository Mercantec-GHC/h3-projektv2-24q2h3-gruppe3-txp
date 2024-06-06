-- AlterTable
ALTER TABLE "Game" ADD COLUMN     "sessionsId" INTEGER;

-- CreateTable
CREATE TABLE "Sessions" (
    "id" SERIAL NOT NULL,
    "ArduinoDevice" TEXT NOT NULL,
    "Account" TEXT,

    CONSTRAINT "Sessions_pkey" PRIMARY KEY ("id")
);

-- CreateIndex
CREATE UNIQUE INDEX "Sessions_ArduinoDevice_key" ON "Sessions"("ArduinoDevice");
