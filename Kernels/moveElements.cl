__kernel void moveElements(__global float *pos,
                            __global float *vel,
                            float x,
                            float y,
                            float z,
                            float amount,
                            float dt,
                            int instances) {
 
    // Get the index of the current element to be processed
    
    int i = get_global_id(0)*3;

    if (i > instances * 3) return;

    float velX = vel[i];
    float velY = vel[i + 1];
    float velZ = vel[i + 2];
    float posX = pos[i];
    float posY = pos[i + 1];
    float posZ = pos[i + 2];

    // Do the operation
    if (amount != 0) {
        float xDist = posX - x;
        float yDist = posY - y;
        float zDist = posZ - z;
        float invDist = 1/(xDist * xDist + yDist * yDist + zDist * zDist);
        velX += invDist * xDist * amount;
        velY += invDist * yDist * amount;
        velZ += invDist * zDist * amount;
    }

    pos[i]      = posX + (velX * dt);
    pos[i + 1]  = posY + (velY * dt);
    pos[i + 2]  = posZ + (velZ * dt);
    vel[i]      = velX * (1 - dt * 0.1);
    vel[i + 1]  = velY * (1 - dt * 0.1);
    vel[i + 2]  = velZ * (1 - dt * 0.1);
    
}