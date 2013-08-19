#include "main.h"
void Engine::save() {
    if(player->destructible->isDead() )
        TCODSystem::deleteFile("game.sav");
    else
    {
        TCODZip zip;
        zip.putInt(map->width);
        zip.putInt(map->height);
        map->save(zip);
        
        //Save the player first to make retrieval easier.
        player->save(zip);
        zip.putInt(actors.size()-1);
        for (Actor **it=actors.begin(); it!=actors.end(); it++) {
            if ( *it != player ) {
                (*it)->save(zip);
            }
        }

        gui->save(zip);
        zip.saveToFile("game.sav");
    }
}

void Map::save(TCODZip &zip) {
    zip.putInt(seed);
    for(int i = 0; i < width * height; i++)
        zip.putInt(tiles[i].explored);
}

void Gui::save(TCODZip &zip) {
    zip.putInt(log.size());
    for(Message ** it = log.begin(); it != log.end(); it++)
    {
        zip.putString( (*it)->text);
        zip.putColor(&(*it)->col);
    }
}

void Actor::save(TCODZip &zip) 
{
    zip.putInt(x);
    zip.putInt(y);
    zip.putInt(ch);
    zip.putColor(&col);
    zip.putString(name);
    zip.putInt(blocking);
    zip.putInt(attacker != NULL);
    zip.putInt(destructible != NULL);
    zip.putInt(ai != NULL);
    zip.putInt(pickable != NULL);
    zip.putInt(container != NULL);
    if ( attacker ) attacker->save(zip);
    if ( destructible ) destructible->save(zip);
    if ( ai ) ai->save(zip);
    if ( pickable ) pickable->save(zip);
    if ( container ) container->save(zip);
}

void Attacker::save(TCODZip &zip)
{
    zip.putFloat(attack_points);
}

void Container::save(TCODZip &zip)
{
    zip.putInt(size);
    zip.putInt(inventory.size());
    for(Actor **it = inventory.begin(); it != inventory.end() ; it++)
        (*it)->save(zip);
}

void Destructible::save(TCODZip &zip) {
    zip.putFloat(max_hp);
    zip.putFloat(hp);
    zip.putFloat(defense);
    zip.putString(corpse_name);
}

void PlayerDestructible::save(TCODZip &zip) {
    zip.putInt(PLAYER);
    Destructible::save(zip);
}
 
void MonsterDestructible::save(TCODZip &zip) {
    zip.putInt(MONSTER);
    Destructible::save(zip);
}

void HealingEntity::save(TCODZip &zip) {
    zip.putInt(HEALING_ENTITY);
    zip.putFloat(amount);
}

void LightningBolt::save(TCODZip &zip) {
    zip.putInt(LIGHTNING_BOLT);
    zip.putFloat(range);
    zip.putFloat(damage);
}

void Confuse::save(TCODZip &zip) {
    zip.putInt(CONFUSE);
    zip.putInt(duration);
    zip.putFloat(range);
}
 
void Fireball::save(TCODZip &zip) {
    zip.putInt(FIREBALL);
    zip.putFloat(range);
    zip.putFloat(damage);   
}

void MonsterAi::save(TCODZip &zip) {
    zip.putInt(MONSTER);
    zip.putInt(move_count);
}

void ConfusedMonsterAi::save(TCODZip &zip) {
    zip.putInt(CONFUSED_MONSTER);
    zip.putInt(duration);
    old_ai->save(zip);
}

void PlayerAi::save(TCODZip &zip) {
    zip.putInt(PLAYER);
}

void Engine::load() {
    if(TCODSystem::fileExists("game.sav")) 
    {
        TCODZip zip;
        zip.loadFromFile("game.sav");
        int width = zip.getInt();
        int height = zip.getInt();
        map = new Map(width, height);
        map->load(zip);
        player = new Actor(0,0,0,NULL,TCODColor::white);
        player->load(zip);
        actors.push(player);
        int num_actors = zip.getInt();
        while(num_actors > 0)
        {
            Actor *actor = new Actor(0,0,0,NULL,TCODColor::white);
            actor->load(zip);
            actors.push(actor);
            num_actors--;
        }

        gui->load(zip);
    } else {
        engine.init();
    }
}

void Map::load(TCODZip &zip)
{
    seed = zip.getInt();
    init(false);
    for(int i = 0 ; i < width * height ; i++)
        tiles[i].explored = (bool) zip.getInt();
}

//Load each message/color pair and log them.
void Gui::load(TCODZip &zip) {
    int num_messages = zip.getInt();
    while(num_messages > 0)
    {
        const char *text = zip.getString();
        TCODColor col = zip.getColor();
        log_message(col, text);
        num_messages--;
    }
}

void Actor::load(TCODZip &zip) {
    x=zip.getInt();
    y=zip.getInt();
    ch=zip.getInt();
    col=zip.getColor();
    name=strdup(zip.getString());
    blocking=zip.getInt();
    bool has_attacker=zip.getInt();
    bool has_destructible=zip.getInt();
    bool has_ai=zip.getInt();
    bool has_pickable=zip.getInt();
    bool has_container=zip.getInt();

    if(has_attacker) 
    {
        attacker = new Attacker(0.0f);
        attacker->load(zip);
    }

    if(has_destructible)
        destructible = Destructible::create(zip);
    
    if(has_ai)
        ai = Ai::create(zip);

    if(has_pickable)
        pickable = Pickable::create(zip);

    if(has_container)
    {
        container = new Container(0);
        container->load(zip);
    }
}

void Attacker::load(TCODZip &zip)
{
    attack_points = zip.getFloat();
}

void Container::load(TCODZip &zip)
{
    size=zip.getInt();
    int num_actors = zip.getInt();
    while ( num_actors > 0 ) {
        Actor *actor = new Actor(0,0,0,NULL,TCODColor::white);
        actor->load(zip);
        inventory.push(actor);
        num_actors--;
    }
}

void Destructible::load(TCODZip &zip)
{
    max_hp = zip.getFloat();
    hp = zip.getFloat();
    defense = zip.getFloat();
    corpse_name = strdup(zip.getString());
}

Destructible *Destructible::create(TCODZip &zip) {
    DestructibleType type=(DestructibleType)zip.getInt();
    Destructible *destructible=NULL;
    switch(type) {
        case MONSTER : destructible=new MonsterDestructible(0,0,NULL); break;
        case PLAYER : destructible=new PlayerDestructible(0,0,NULL); break;
    }
    destructible->load(zip);
    return destructible;
}

void Confuse::load(TCODZip &zip) {
    duration=zip.getInt();
    range=zip.getFloat();
}

void LightningBolt::load(TCODZip &zip) {
    range=zip.getFloat();
    damage=zip.getFloat();
}

void HealingEntity::load(TCODZip &zip) {
    amount=zip.getFloat();
}

Pickable *Pickable::create(TCODZip &zip) {
    PickableType type=(PickableType)zip.getInt();
    Pickable *pickable=NULL;
    switch(type) {
        case HEALING_ENTITY : pickable=new HealingEntity(0); break;
        case LIGHTNING_BOLT : pickable=new LightningBolt(0,0); break;
        case CONFUSE : pickable=new Confuse(0,0); break;
        case FIREBALL : pickable=new Fireball(0,0); break;
    }
    pickable->load(zip);
    return pickable;
}

void MonsterAi::load(TCODZip &zip) {
    move_count=zip.getInt();
}

 void ConfusedMonsterAi::load(TCODZip &zip) {
    duration = zip.getInt();
    old_ai = Ai::create(zip);
}

void PlayerAi::load(TCODZip &zip) {
}

Ai *Ai::create(TCODZip &zip) {
    AiType type=(AiType)zip.getInt();
    Ai *ai=NULL;
    switch(type) {
        case PLAYER : ai = new PlayerAi(); break;
        case MONSTER : ai = new MonsterAi(); break;
        case CONFUSED_MONSTER : ai = new ConfusedMonsterAi(0,NULL); break;
    }
    ai->load(zip);
    return ai;
}
