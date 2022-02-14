CREATE TABLE t_user_info (
        id serial PRIMARY KEY,
        user_name VARCHAR(200) NOT NULL UNIQUE,
        password VARCHAR(200) NOT NULL,
        true_name VARCHAR(100) DEFAULT '',
        nick_name VARCHAR(100) DEFAULT '',
        gender INT DEFAULT 0, -- 0:unknown 1:male 2:female
        age INT DEFAULT 0,
        email VARCHAR(100) DEFAULT '',
        address VARCHAR(200) DEFAULT '',
        phone VARCHAR(20) DEFAULT '',
        relationship INT DEFAULT 0, -- 0:unknown 1:single 2:in a relationship
        profile_image VARCHAR(300) DEFAULT '', 
        work VARCHAR(100) DEFAULT '',
        interest VARCHAR(200) DEFAULT '',
        create_time INT NOT NULL,
        last_login_time INT NOT NULL,
        create_ip VARCHAR(25) DEFAULT '0.0.0.0',
        last_login_ip VARCHAR(25) DEFAULT '0.0.0.0',
        login_count INT NOT NULL DEFAULT 0
);


CREATE TABLE t_blog (
        id serial PRIMARY KEY, 
        user_id INT NOT NULL,
        content TEXT DEFAULT '',
        create_time INT NOT NULL,
        update_time INT NOT NULL,
        modified_count INT NOT NULL DEFAULT 0,
        title VARCHAR(200) NOT NULL,
        sub_title VARCHAR(200) NOT NULL,
        tags VARCHAR(1000) NOT NULL,
        images TEXT DEFAULT ''
);

CREATE TABLE t_video (
        id serial PRIMARY KEY,
        video_hash VARCHAR(150) UNIQUE, 
        video_name VARCHAR(400) NOT NULL,
        video_size INT NOT NULL DEFAULT 0,
        state INT NOT NULL DEFAULT 0, -- state of this record. 0: unfinished, 1: finished
        added_time INT NOT NULL DEFAULT 0,
        completed_time INT NOT NULL DEFAULT 0,
        video_link VARCHAR(400) DEFAULT '',
        profile_link VARCHAR(400) DEFAULT '',
        score INT NOT NULL DEFAULT 0,
        tags VARCHAR(500) DEFAULT ''
);