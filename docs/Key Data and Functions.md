# Key Data and Functions

All information can be obtained through reverse engineering.

## Warning

> This document is only for *Plants vs. Zombies **1.0.0.1051 CHINESE*** version.
>
> The *MD5* of `PlantsVsZombies.exe` is `37B729B4056131722A556E646AC915E9`.

## Data

### Slots

Set a breakpoint at `0x004897B2`. Each time the game is interrupted while loading a new level, the 4-bit content of `[ESP]` will be the address of the  slot structure that the game is displaying currently.

```c++
/**
 * @brief The slot.
 */
struct Slot {
    std::byte unknown1[8];

    /**
     * @brief The X-coordinate on the screen.
     * @par Offset
     * 0x8
     */
    std::int32_t screen_pos_x;

    /**
     * @brief The Y-coordinate on the screen.
     * @par Offset
     * 0xC
     */
    std::int32_t screen_pos_y;

    std::byte unknown2[36];

    /**
     * @brief The item ID.
     * @details
     * If the type of the slot is zombie, this field is equal to the zombie ID plus 0x3C.
     * If the type of the slot is plant, this field is equal to the plant ID.
     *
     * @par Offset
     * 0x34
     */
    std::int32_t id;
};
```

### The Level

- **The global base address**

  ```assembly
  mov ebx, 006A9F38h
  ```

  - **The base address of the level environment**

    ```assembly
    mov ebx, dword ptr [006A9F38h]
    mov ebx, dword ptr [ebx + 768h]
    ```

    - **The amount of sun**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 5560h]
      ```

    - **The number of zombies**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 0A0h]
      ```

    - **The number of planted plants**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 0B0h]
      ```

    - **The base address of all planted plant structures**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov ebx, dword ptr [ebx + 0ACh]
      ```

      The type of this structure is `PlantedPlant`.

    - **The countdown**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 5600h]
      ```

### Plants

- **Static plant information**

  ```c++
  /**
   * @brief Static plant information.
   * @par Binary Address
   * 0x0069F2B0
   */
  struct Plant {
      /**
       * @brief The ID.
       * @par Offset
       * 0x0
       */
      std::int32_t id;

      std::byte unknown1[4];

      /**
       * @brief The image resource.
       * @par Offset
       * 0x8
       */
      std::int32_t ui;

      std::byte unknown2[4];

      /**
       * @brief The cost to plant.
       * @par Offset
       * 0x10
       */
      std::int32_t sun_cost;

      /**
       * @brief The recharge time.
       * @par Offset
       * 0x14
       */
      std::int32_t recharge_time;

      std::byte unknown3[12];

      /**
       * @brief The name.
       * @par Offset
       * 0x20
       */
      char* name;
  };
  ```

  The base address of all structures is `0x0069F2B0`.

- **Planted plants in a level**

  ```c++
  /**
   * @brief The planted plant in a level.
   */
  struct PlantedPlant
  {
      std::byte unknown1[321];

      /**
       * @brief If the plant is invalid.
       * @details If this field is `true`, the plant will be removed.
       *
       * @par Offset
       * 0x141
       */
      bool invalid;

      std::byte unknown2[10];
  };
  ```

- **The plant ID**

  |  ID  |       Name       |
  | :--: | :--------------: |
  |  0   |   *Peashooter*   |
  |  1   |   *Sunflower*    |
  |  2   |  *Cherry Bomb*   |
  |  3   |    *Wall-nut*    |
  |  4   |  *Potato Mine*   |
  |  5   |    *Snow Pea*    |
  |  6   |    *Chomper*     |
  |  7   |    *Repeater*    |
  |  8   |  *Puff-shroom*   |
  |  9   |   *Sun-shroom*   |
  |  10  |  *Fume-shroom*   |
  |  11  |  *Grave Buster*  |
  |  12  |  *Hypno-shroom*  |
  |  13  | *Scaredy-shroom* |
  |  14  |   *Ice-shroom*   |
  |  15  |  *Doom-shroom*   |
  |  16  |    *Lily Pad*    |
  |  17  |     *Squash*     |
  |  18  |  *Threepeater*   |
  |  19  |  *Tangle Kelp*   |
  |  20  |    *Jalapeno*    |
  |  21  |   *Spikeweed*    |
  |  22  |   *Torchwood*    |
  |  23  |    *Tall-nut*    |
  |  24  |   *Sea-shroom*   |
  |  25  |    *Plantern*    |
  |  26  |     *Cactus*     |
  |  27  |     *Blover*     |
  |  28  |   *Split Pea*    |
  |  29  |   *Starfruit*    |
  |  30  |    *Pumpkin*     |
  |  31  | *Magnet-shroom*  |
  |  32  |  *Cabbage-pult*  |
  |  33  |   *Flower Pot*   |
  |  34  |  *Kernel-pult*   |
  |  35  |  *Coffee Bean*   |
  |  36  |     *Garlic*     |
  |  37  | *Umbrella Leaf*  |
  |  38  |    *Marigold*    |
  |  39  |   *Melon-pult*   |
  |  40  |  *Gatling Pea*   |
  |  41  | *Twin Sunflower* |
  |  42  |  *Gloom-shroom*  |
  |  43  |    *Cattail*     |
  |  44  |  *Winter Melon*  |
  |  45  |  *Gold Magnet*   |
  |  46  |   *Spikerock*    |
  |  47  |   *Cob Cannon*   |
  |  48  |    *Imitater*    |

### Zombies

- **The zombie ID**

  |  ID  |           Name           |
  | :--: | :----------------------: |
  |  0   |         *Zombie*         |
  |  1   |      *Flag Zombie*       |
  |  2   |    *Conehead Zombie*     |
  |  3   |  *Pole Vaulting Zombie*  |
  |  4   |   *Buckethead Zombie*    |
  |  5   |    *Newspaper Zombie*    |
  |  6   |   *Screen Door Zombie*   |
  |  7   |    *Football Zombie*     |
  |  8   |     *Dancing Zombie*     |
  |  9   |     *Backup Dancer*      |
  |  10  |   *Ducky Tube Zombie*    |
  |  11  |     *Snorkel Zombie*     |
  |  12  |        *Zomboni*         |
  |  13  |  *Zombie Bobsled Team*   |
  |  14  |  *Dolphin Rider Zombie*  |
  |  15  | *Jack-in-the-Box Zombie* |
  |  16  |     *Balloon Zombie*     |
  |  17  |     *Digger Zombie*      |
  |  18  |      *Pogo Zombie*       |
  |  19  |      *Zombie Yeti*       |
  |  20  |     *Bungee Zombie*      |
  |  21  |     *Ladder Zombie*      |
  |  22  |    *Catapult Zombie*     |
  |  23  |       *Gargantuar*       |
  |  24  |          *Imp*           |
  |  25  |      *Dr. Zomboss*       |

## Functions

### Game Running

- **Start a level.**

  ```c++
  /**
   * @brief Start a level.
   *
   * @param level_id  A level ID.
   * @param reset     It's `true` when starting a new level, `false` when loading the last progress.
   *
   * @par Binary Address
   * 0x0044F560
   */
  void StartLevel(
      std::int32_t level_id,
      bool reset);
  ```

- **Terminate the level.**

  ```c++
  /**
   * @brief Terminate the level.
   *
   * @param level_env The level environment.
   * @param unknown   It's always 0.
   *
   * @par Binary Address
   * 0x00413400
   */
  void EndLevel(
      void* level_env,
      std::int32_t unknown);
  ```

- **Subtract the amount of sun.**

  ```c++
  /**
   * @brief Subtract the amount of sun.
   *
   * @param count The amount to be subtracted, stored in `EBX`.
   * @return `true` if the amount of sun is enough, otherwise `false`, stored in `AL`.
   *
   * @par Binary Address
   * 0x0041BA60
   */
  bool SubtractSun(
      std::int32_t count);
  ```

- **Disable automatic pause.**

  ```assembly
  .text:0044F475  mov ecx, esi
  .text:0044F477  pop esi
  .text:0044F478  jmp sub_4502C0
  ```

  Change `jmp sub_4502C0` to `ret`.

- **Disable the running menu.**

  ```assembly
  .text:004500FC  cmp eax, esi
  .text:004500FE  mov [esp + 18h], esi
  .text:00450102  jz  short loc_450112
  ```

  Change `jz short loc_450112` to ` jmp 0045016Ah`.

- **Load the last level.**

  ```assembly
  .text:004336E6  mov esi, ecx
  .text:004336E8  jnz short loc_433766
  ```

  Change `jnz short loc_433766` to `nop`.

- **Be able to run multiple game processes simultaneously.**

  ```assembly
  .text:00553F10  call ds:GetLastError
  .text:00553F16  cmp  eax, ERROR_ALREADY_EXISTS
  .text:00553F1B  jnz  short loc_553F29
  ```

  Change `jnz` to `jmp`.

### Plants

- **Check if a plant can be planted in a certain location.**

  ```c++
  /**
   * @brief Check if a plant can be planted in a certain location.
   *
   * @param level_env The level environment.
   * @param pos_x     The X coordinate of the target location.
   * @param pos_y     The Y coordinate of the target location, stored in `EAX`.
   * @param plant_id  A plant ID.
   * @return 0 if the plant can be planted, otherwise the error code, stored in `EAX`.
   *
   * @par Binary Address
   * 0x0040E020
   */
  std::int32_t CheckPlantingConflit(
      void* level_env,
      std::int32_t pos_x,
      std::int32_t pos_y,
      std::int32_t plant_id);
  ```

- **Create a plant.**

  ```c++
  /**
   * @brief Create a plant.
   *
   * @param level_env The level environment.
   * @param pos_x     The X coordinate of the target location.
   * @param pos_y     The Y coordinate of the target location, stored in `EAX`.
   * @param plant_id  A plant ID.
   * @param unknown   It's always -1.
   * @return Unknown.
   *
   * @par Binary Address
   * 0x0040D120
   */
  std::int32_t CreatePlant(
      void* level_env,
      std::int32_t pos_x,
      std::int32_t pos_y,
      std::int32_t plant_id,
      std::int32_t unknown);
  ```

- **Remove a plant.**

  ```c++
  /**
   * @brief Remove a plant.
   *
   * @param plant A plant.
   *
   * @par Binary Address
   * 0x004679B0
   */
  void RemovePlant(
      void* plant);
  ```

- **Remove all plants.**

  ```assembly
  .text:0041BB28  cmp [eax + 141h], bl
  .text:0041BB2E  jz  short loc_41BAE0
  ```

  Change `jz short loc_41BAE0` to `nop`.

### Zombies

- **Create a zombie.**

  ```c++
  /**
   * @brief Create a zombie.
   *
   * @param level_env The level environment, stored in `ECX`.
   * @param zombie_id A zombie ID.
   * @param pos_x     The X coordinate of the target location.
   * @param pos_y     The X coordinate of the target location, stored in `EAX`.
   * @return Unknown.
   *
   * @par Binary Address
   * 0x0042A0F0
   */
  std::int32_t CreateZombie(
      void* level_env,
      std::int32_t zombie_id,
      std::int32_t pos_x,
      std::int32_t pos_y);
  ```

- **Stop generating zombies.**

  ```assembly
  .text:0040DDD5  cmp  [esi + 0A0h], eax
  .text:0040DDDB  push edi
  .text:0040DDDC  jb   short loc_40DDE8
  ```

  Change `jb short loc_40DDE8` to `nop`.

- **Make zombies walk backwards.**

  ```assembly
  .text:0052AB21  cmp dword ptr [esi + 28h], 28h
  .text:0052AB25  jz  short loc_52AB30
  ```

  Change `jz` to `jmp`.

- **Kill all zombies.**

  ```assembly
  .text:0052AB3E  jnz loc_52ABE8
  .text:0052AB44  cmp dword ptr [esi + 6Ch], 0FFFFFFFCh
  ```

  Change these two instructions to `mov dword ptr [esi + 28h], 3`.

## Contact

- ***Chen Zhenshuo***

  > ***GitHub***: https://github.com/czs108
  >
  > ***E-Mail***: chenzs108@outlook.com
  >
  > ***WeChat***: chenzs108

- ***Liu Guowen***

  > ***GitHub***: https://github.com/lgw1995
  >
  > ***E-Mail***: liu.guowen@outlook.com