# Simplified MySQL

[TOC]



## 基础类型操作

```mysql
# 字符串以xxx开头
[...] where str like 'xxx%';

# 得到date格式数据的年月日
year(some_date); # month day hour minute second
```

## CREATE 

```mysql
create database if not exists test;
drop database if exists test;

# 其他的关键字
'''
unique ~ 
'''

create table Project
(
  PID       int(11)     not null primary key, # auto_increment
  Pname     varchar(32) not null,
  city      varchar(32) not null,
  principal varchar(32) not null
);

create table Bank
(
  BID       int(11)     not null primary key,
  Bname     varchar(32) not null,
  city      varchar(32) not null
);

create table Loan
(
  PID    int(11) not null,
  BID    int(11) not null,
  money1 int(11) not null,
  Bdate  date    not null,
  Rdate  date,
  constraint CProject foreign key(PID) references Project(PID),
  constraint CBank    foreign key(BID) references Bank(BID),
  constraint C1       check(Rdate >= Bdate or Rdate = null)
);

'''
constraint ${约束名} foreign key(${约束变量}) references ${被引用表}(被引用变量);
'''
```

## INSERT

```mysql
insert into Bank values (1, '中国银行', '上海');
insert into Project set PID=1, Pname='MIPS54-CPU', city='北京', principal='李润中';
```

## SELECT

```mysql
# 第1大&第3大的money1
(
  select * from Loan
	order by money1 desc 
	limit 0, 1
) union (
  select * from Loan
	order by money1 desc 
	limit 2, 1
);

# between
select * from Loan where money1 between 200000 and 300000;
```

```mysql
# max(), min(), count(), group_concat(), 
select group_concat(PID)
from Loan group by BID
having count(PID) > 1;
```

## UPDATE

```mysql
# 如果直接在Update XXX时，内部的变量直接用，不用from，用from就会报错；
update Loan set money1 = (select avg(money1)) where money1 = 0;
```

## JOIN

![](https://i.loli.net/2019/11/05/5RA8FGmXi4gTjyL.jpg)

- `join`默认是`inner join `；

### INNER JOIN

```mysql
# 不同集合的行多对多匹配后，返回on后面条件相同的行；
select * from Bank b
inner join Project p
on b.city = p.city;  # 重点是这个on

# 上面的命令还是会有2个重复的city列
# 如果想把这个相同值的city结合的话，可以使用using函数
# @使用using函数的时候就不要on了
select * from Bank b
inner join Project p
using(city);
```

### 外连接

- `left join`: 
  - 返回左表的所有行，即使右表中没有匹配的行；（等价于`left outer join`）
  - `from`后接的表是主表；
  - 存在匹配项就返回右侧匹配项，如果右侧不存在匹配项则直接补`null`；
- `right join`: 同上（左右对换）；

```mysql
select * from Project p
left join Bank b
on p.PID = b.BID-100;

select * from Project p
right join Bank b
on p.PID = b.BID-100;
```

### 全连接

基于上述集合，直接`union`。

### 自然连接

> 自动结合所有重复命名的列。

```mysql
select * from Project natural join Bank;
```

## 触发器

- 记得begin-end；
- `end if`后记得结束符；
- 

```mysql
delimiter $$
create trigger trig before update on Loan
	for each row
	begin
		if     new.money1 < 0   then
			set  new.money1 = 0;
		elseif new.money1 > 100 then
			set  new.money1 = 100;
		end if;# END IF后有分号
	end$$
delimiter ;
```

## Normal Func

- `char_length(s)`字符串长度
- `concat(s1, s2, s3)`字符串拼接

