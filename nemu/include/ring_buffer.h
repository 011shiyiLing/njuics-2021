typedef struct{
  unsigned *buff;
  int length;
  int head;
  int tail;
}ring_buffer_t;

ring_buffer_t *ring_buffer_create_init(int length)
{
    ring_buffer_t *tmp_ring_buffer = (ring_buffer_t *)malloc(sizeof(ring_buffer_t));
    tmp_ring_buffer->buff = (unsigned *)malloc(length);
    memset(tmp_ring_buffer->buff, 0, length);

    //init
    tmp_ring_buffer->head = 0;
    tmp_ring_buffer->tail = 0;
    tmp_ring_buffer->length = length;

    return tmp_ring_buffer;
}


//write a byte
void write_ring_buffer(ring_buffer_t *ring_buffer, unsigned data)
{
    ring_buffer->buff[ring_buffer->head] = data;
    ring_buffer->head++;

    if(ring_buffer->head == ring_buffer->length)
    {
	    ring_buffer->head = 0;
    }
}

//read a byte
unsigned read_ring_buffer_byte(ring_buffer_t *ring_buffer)
{
    if(ring_buffer->head == ring_buffer->tail)
    {
        return -1;
    }
    
    unsigned data = ring_buffer->buff[ring_buffer->tail];
    ring_buffer->tail++;

    if(ring_buffer->tail == ring_buffer->length){
	    ring_buffer->tail = 0;
    }

    return data;
}

