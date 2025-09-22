#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    // We write the file name to filename
    char filename[1024];
    {
        const char message[] = "Input filename: ";
        write(STDOUT_FILENO, message, sizeof(message));

        int bytes = read(STDIN_FILENO, filename, sizeof(filename) - 1);
        if (bytes <= 0)
        {
            const char err_msg[] = "error: Impossible to read from user\n";
            write(STDERR_FILENO, err_msg, sizeof(err_msg));
            exit(EXIT_FAILURE);
        }
        filename[bytes - 1] = '\0';
    }

    // Creating a pipe for interprocess communication
    int file_descriptor[2];
    if (pipe(file_descriptor) == -1)
    {
        const char err_msg[] = "error: Impossible to create pipe\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg));
        exit(EXIT_FAILURE);
    }

    pid_t child = fork();
    switch (child) 
    {
        case -1:
        { // We cannot spawn a child process
            const char err_msg[] = "error: impossible to spawn new process\n";
            write(STDERR_FILENO, err_msg, sizeof(err_msg));
            exit(EXIT_FAILURE);
        } break;

        case 0:
        { // We're in child process
            close(file_descriptor[0]);
            dup2(file_descriptor[1], STDOUT_FILENO);
            close(file_descriptor[1]);

            int file = open(filename, O_RDONLY);
            if (file == -1)
            {
                const char err_msg[] = "error: Error opening file\n";
                write(STDERR_FILENO, err_msg, sizeof(err_msg));
                exit(EXIT_FAILURE);
            }

            // Duplicate the file descriptor as an input stream for the child process
            dup2(file, STDIN_FILENO);
            close(file);

            int status = execl("./child", "child", NULL);

			if (status == -1) {
				const char err_msg[] = "error: failed to exec into new exectuable image\n";
				write(STDERR_FILENO, err_msg, sizeof(err_msg));
				exit(EXIT_FAILURE);
			}
        }

        default:
        { // We are in parent's process
            close(file_descriptor[1]);


            // Read output from child process
            char buff[1024];
            int status;
            int bytes;
            while ((bytes = read(file_descriptor[0], buff, sizeof(buff))) > 0)
            {
                write(STDOUT_FILENO, buff, bytes);
            }

            close(file_descriptor[0]);

            wait(&status);

            // Checking the exit status of a child process
            if (WIFEXITED(status))
            {
                if (WEXITSTATUS(status) != 0) 
                {
                    const char err_msg[] = "error: the child process terminated with an error\n";
                    write(STDERR_FILENO, err_msg, sizeof(err_msg));
                    exit(EXIT_FAILURE);
                }
                else 
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else
            {
                const char err_msg[] = "error: The process has not completed its execution\n";
                write(STDERR_FILENO, err_msg, sizeof(err_msg));
                exit(EXIT_FAILURE);
            }
        }
    }
}