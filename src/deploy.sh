if scp cat/s21_cat grep/s21_grep hakearie@172.24.116.8:~/ ; then
        ssh hakearie@172.24.116.8 "echo $hakearie_PASSWORD | sudo -S mv s21_cat s21_grep /usr/local/bin"
        echo -e "\e[32mSuccesfully copy artifacts to /usr/local/bin\e[0m"
    else
        echo -e "\e[31mCAN'T COPY ARTIFACTS\e[0m"
        exit 1
      fi


