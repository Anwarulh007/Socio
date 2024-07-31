#include <stdio.h>

typedef struct
{
    char username[100];
    int likes;
    int comments;
    int chats;
    int tagged_photos;
    int comments_mentioned;
    int points;
} Data;

int main(void)
{
    FILE *file;
    file = fopen("C:\\PIYUSH BYAHUT DATA\\college program\\latest-activities.csv", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }
    Data data[100];
    Data temp;
    int read = 0;
    int records = 0;

    do
    {
        read = fscanf(file, "%50[^,],%d,%d,%d,%d,%d\n", data[records].username, 
                                                        &data[records].likes, 
                                                        &data[records].comments, 
                                                        &data[records].chats, 
                                                        &data[records].tagged_photos, 
                                                        &data[records].comments_mentioned);

        if (read == 6)
            records++;
        if (read != 6 && !feof(file))
        {
            printf("File format incorrect.\n");
            return 1;
        }
        if (ferror(file))
        {
            printf("Error reading file.\n");
            return 1;
        }
    } while (!feof(file));

    fclose(file);

    printf("\n%d records read.\n\n", records);
    for (int i = 0; i < records; i++)
        printf("%s %d %d %d %d %d\n", data[i].username, 
                                      data[i].likes, 
                                      data[i].comments, 
                                      data[i].chats, 
                                      data[i].tagged_photos, 
                                      data[i].comments_mentioned);
    printf("\n");

    for (int i = 0; i < records; i++)
    {
        data[i].points = data[i].likes + 
                         data[i].comments * 1.5 + 
                         data[i].chats / 5 + 
                         data[i].tagged_photos * 2 + 
                         data[i].comments_mentioned * 3;

        printf("%s %d\n", data[i].username, data[i].points);
    }

    for (int i = 1; i < records; i++)
        for (int j = 0; j < records - i; j++)
        {
            if (data[j].points < data[j + 1].points)
            {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }

    printf("\nSorted data:\n");
    for (int i = 0; i < records; i++)
        printf("%s %d\n", data[i].username, data[i].points);

    printf("\nThese are your 5 closest friends:\n");
    for (int i = 0; i < 5; i++)
        printf("%s\n", data[i].username);

    return 0;
}